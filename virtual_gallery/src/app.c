#include "app.h"

#include <stdlib.h>

#include <SDL2/SDL_image.h>

void init_app(App *app, int width, int height) {
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
            "Virtual Gallery",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    inited_loaders = IMG_Init(img_flags);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl() {
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height) {
    int x, y, w, h;
    double ratio;

    ratio = (double) width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int) ((double) height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    } else {
        w = width;
        h = (int) ((double) width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
            -.08, .08,
            -.06, .06,
            .1, 2000
    );
}

void handle_app_events(App *app) {
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        app->is_running = false;
                        break;
                    case SDL_SCANCODE_W:
                        set_camera_speed(&(app->camera), 20);
                        break;
                    case SDL_SCANCODE_S:
                        set_camera_speed(&(app->camera), -20);
                        break;
                    case SDL_SCANCODE_A:
                        set_camera_side_speed(&(app->camera), 20);
                        break;
                    case SDL_SCANCODE_D:
                        set_camera_side_speed(&(app->camera), -20);
                        break;
                    case SDL_SCANCODE_SPACE:
                        set_camera_speed_z(&(app->camera), 10);
                        break;
                    case SDL_SCANCODE_LSHIFT:
                        set_camera_speed_z(&(app->camera), -10);
                        break;
                    case SDL_SCANCODE_1:
                        setBrightness(&(app->scene), -3.0f);
                        break;
                    case SDL_SCANCODE_2:
                        setBrightness(&(app->scene), 3.0f);
                        break;
                    case SDL_SCANCODE_M:
                        app->scene.animation_flag = !app->scene.animation_flag;
                        set_camera_for_animation(app);
                        break;
                    case SDL_SCANCODE_TAB:
                        app->scene.guide_flag = !app->scene.guide_flag;
                        break;
                    case SDL_SCANCODE_3:
                        toggle_fog(&(app->scene));
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_S:
                        set_camera_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_D:
                        set_camera_side_speed(&(app->camera), 0);
                        break;
                    case SDL_SCANCODE_SPACE:
                    case SDL_SCANCODE_LSHIFT:
                        set_camera_speed_z(&(app->camera), 0);
                    case SDL_SCANCODE_1:
                    case SDL_SCANCODE_2:
                        setBrightness(&(app->scene), 0.0f);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                is_mouse_down = true;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x, &y);
                if (is_mouse_down) {
                    rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
                }
                mouse_x = x;
                mouse_y = y;
                break;
            case SDL_MOUSEBUTTONUP:
                is_mouse_down = false;
                break;
            case SDL_QUIT:
                app->is_running = false;
                break;
            default:
                break;
        }
    }
}

void update_app(App *app) {
    double current_time;
    double elapsed_time;

    current_time = (double) SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), elapsed_time);

    if (app->scene.animation_path >= 15.0f) {
        app->camera = app->saved_camera;
    } else if (app->scene.animation_path <= -3.0f) {
        app->camera = app->saved_camera;
    }
    if (app->scene.animation_flag) {
        if (app->scene.animation_direction) {
            app->camera.rotation.z += 0.01f;
        } else {
            app->camera.rotation.z -= 0.01f;
        }
    }
}

void render_app(App *app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }
    

    if (app->scene.guide_flag) {
        show_guide(app->scene.guide_texture);
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app) {
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

void set_camera_for_animation(App *app) {
    if (app->scene.animation_flag) {
        app->saved_camera = app->camera;
        app->camera.position.x = 30.0f;
        app->camera.position.y = 40.0f;
        app->camera.position.z = 10.0f;

        app->camera.rotation.x = 20.0f;
        app->camera.rotation.y = 15.0f;
        if (app->scene.animation_direction) {
            // view start position 
            app->camera.rotation.z = 245.0f;

        } else {
            // view end position 
            app->camera.rotation.z = 269.04f;
        }

    } else {
        app->camera = app->saved_camera;
    }
}
