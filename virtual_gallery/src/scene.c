#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

#include <GL/glu.h>
#include <math.h>

void init_scene(Scene *scene) {
    load_models_init_scene(scene);
    load_textures_init_scene(scene);

    scene->material.ambient.red = 0.0f;
    scene->material.ambient.green = 0.0f;
    scene->material.ambient.blue = 0.0f;

    scene->material.diffuse.red = 1.0f;
    scene->material.diffuse.green = 1.0f;
    scene->material.diffuse.blue = 0.0f;

    scene->material.specular.red = 0.0f;
    scene->material.specular.green = 0.0f;
    scene->material.specular.blue = 0.0f;

    scene->material.shininess = 0.0f;
    scene->brightness = 0.0f;

    scene->guide_flag = false;
    scene->fog_flag = false;

    scene->animation_path = -2.0f;
    scene->animation_flag = false;
    scene->animation_direction = true;

    scene->diffuse[0] = 1.0f;
    scene->diffuse[1] = 1.0f;
    scene->diffuse[2] = 1.0f;
    
}

void load_models_init_scene(Scene *scene) {
    load_model(&(scene->floor), "assets/models/floor3.obj");
    load_model(&(scene->wall1), "assets/models/wall1.obj");

    load_model(&(scene->img1), "assets/models/img1.obj");

    load_model(&(scene->ufo), "assets/models/ufo.obj");
}

void load_textures_init_scene(Scene *scene) {
    scene->skybox_texture = load_texture("assets/textures/james_webb.jpg");
    scene->floor_texture = load_texture("assets/textures/floor.jpg");
    scene->wall_texture = load_texture("assets/textures/wall_texture.jpg");

    scene->img1_texture = load_texture("assets/textures/img1.png");
    scene->img2_texture = load_texture("assets/textures/img2.png");
    scene->img3_texture = load_texture("assets/textures/img3.png");
    scene->img4_texture = load_texture("assets/textures/img4.png");
    scene->img5_texture = load_texture("assets/textures/img5.png");
    scene->img6_texture = load_texture("assets/textures/img6.png");

    scene->ufo_texture = load_texture("assets/textures/ufo_texture.png");
    scene->guide_texture = load_texture("assets/textures/guide.png");
    //scene->pendulum_texture = load_texture("assets/textures/ufo_texture.png");

}

void load_skybox(Scene scene) {
    glDisable(GL_LIGHTING);

    glBindTexture(GL_TEXTURE_2D, scene.skybox_texture);

    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;
    int n_slices, n_stacks;
    double radius;
    int i, k;
    n_slices = 15;
    n_stacks = 15;
    radius = 100;

    glPushMatrix();

    glScaled(radius, radius, radius);

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double) i / n_stacks;
        v2 = (double) (i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double) k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1) - 0.25;
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2) - 0.25;
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);
        }
    }

    glEnd();

    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void load_objects(Scene scene) {

    for(int i = 0; i < 3; i++) {    
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, scene.wall_texture);
        glTranslatef(0.0f, i * -40.0f, -0.5f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        draw_model(&(scene.wall1));
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    for(int i = 0; i < 3; i++) {
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, scene.wall_texture);
        glTranslatef(-40.0f, i * -30.0f, -0.5f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        draw_model(&(scene.wall1));
        
        glPopMatrix();
    }

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.floor_texture);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -3.5f, 0.0f);
    glScalef(6.0f, 6.0f, 6.0f);
    draw_model(&(scene.floor));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.img1_texture);
    glTranslatef(-0.4f, -40.0f, -0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    draw_model(&(scene.img1));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.img2_texture);
    glTranslatef(-0.4f, 0.0f, -0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    draw_model(&(scene.img1));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.img3_texture);
    glTranslatef(-0.4f, -80.0f, -0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    draw_model(&(scene.img1));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.img4_texture);
    glTranslatef(-39.6f, -30.0f, -0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    draw_model(&(scene.img1));
    glPopMatrix();    

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.img5_texture);
    glTranslatef(-39.6f, 0.0f, -0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    draw_model(&(scene.img1));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.img6_texture);
    glTranslatef(-39.6f, -60.0f, -0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glScalef(2.0f, 2.0f, 2.0f);
    draw_model(&(scene.img1));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene.ufo_texture);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-30.0f, 30.0f, scene.animation_path);
    draw_model(&(scene.ufo));
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, scene.skybox_texture);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-30.0f, 15.0f, -25.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    draw_model(&(scene.ufo));
    glPopMatrix();
/*
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, scene.pendulum_texture);
        glTranslatef(i * 10.0f, i * -30.0f, 0.0f);  // Translate the pendulum to its position
        glRotatef(scene.pendulums[i].angle, 0.0f, 0.0f, 1.0f);  // Rotate the pendulum based on its angle
        draw_pendulum(scene.pendulums[i].length);  // Draw the pendulum
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
*/
}
/*
void draw_pendulum(float length) {
    const float radius = 2.0f;
    const int slices = 16;
    const int stacks = 16;

    GLUquadricObj *cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluCylinder(cylinder, radius, radius, length, slices, stacks);

    GLUquadricObj *sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluSphere(sphere, radius, slices, stacks);

    gluDeleteQuadric(cylinder);
    gluDeleteQuadric(sphere);
}
*/
void set_lighting(const Scene *scene) {
    float ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float diffuse_light[] = {scene->diffuse[0], scene->diffuse[1], scene->diffuse[2], 1.0f};
    float specular_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float position[] = {0.0f, 0.0f, 50.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material *material) {
    float ambient_material_color[] = {
            material->ambient.red,
            material->ambient.green,
            material->ambient.blue
    };

    float diffuse_material_color[] = {
            material->diffuse.red,
            material->diffuse.green,
            material->diffuse.blue
    };

    float specular_material_color[] = {
            material->specular.red,
            material->specular.green,
            material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene *scene, double time) {
    if (scene->animation_flag) {
        if (scene->animation_direction) {
            scene->animation_path += 1.5f * (float) time;
            if (scene->animation_path >= 15.0f) {
                scene->animation_flag = false;
                scene->animation_direction = false;
            }
        } else {
            scene->animation_path -= 1.5f * (float) time;
            if (scene->animation_path <= -3.0f) {
                scene->animation_flag = false;
                scene->animation_direction = true;
            }
        }
    }

    scene->diffuse[0] += scene->brightness * (float) time;
    scene->diffuse[1] += scene->brightness * (float) time;
    scene->diffuse[2] += scene->brightness * (float) time;
}

void render_scene(Scene *scene) {
    set_material(&(scene->material));
    set_lighting(scene);
    load_skybox(*scene);
    load_objects(*scene);
    set_fog(scene);
}

void draw_origin(void) {
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);

    glEnd();
}

void show_guide(GLuint texture) {
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3d(-2.5f, 2.0f, -3.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3d(2.5f, 2.0f, -3.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3d(2.5f, -2.0f, -3.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3d(-2.5f, -2.0f, -3.0f);
    glEnd();


    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void toggle_fog(Scene* scene) {
    scene->fog_flag = !scene->fog_flag;
}

void set_fog(Scene* scene) {
    if (scene->fog_flag) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 50.0f);
        glFogf(GL_FOG_END, 100.0f);
        GLfloat fog_color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glFogfv(GL_FOG_COLOR, fog_color);
    } else {
        glDisable(GL_FOG);
    }
}

void setBrightness(Scene *scene, float brightness) {
    scene->brightness = brightness;
}