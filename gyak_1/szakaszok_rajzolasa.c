#include <SDL2/SDL.h>
#include <OpenGL/gl.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_LINE_COUNT 100

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    Point start;
    Point end;
    float color[3];
} Line;

Line lines[MAX_LINE_COUNT];
int lineCount = 0;
float currentColor[3] = {1.0f, 1.0f, 1.0f}; // default color - white

void drawLine(Line line) {
    glColor3fv(line.color);
    glBegin(GL_LINES);
    glVertex2f(line.start.x, line.start.y);
    glVertex2f(line.end.x, line.end.y);
    glEnd();
}

void drawRect(Point topLeft, Point bottomRight, float color[3]) {
    glColor3fv(color);
    glBegin(GL_QUADS);
    glVertex2f(topLeft.x, topLeft.y);
    glVertex2f(bottomRight.x, topLeft.y);
    glVertex2f(bottomRight.x, bottomRight.y);
    glVertex2f(topLeft.x, bottomRight.y);
    glEnd();
}

void handleMouseEvent(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        Point mousePosition = {event.button.x, SCREEN_HEIGHT - event.button.y};
        if (lineCount < MAX_LINE_COUNT) {
            Line newLine = {lines[lineCount].start, mousePosition, currentColor};
            lines[lineCount] = newLine;
            lineCount++;
        }
    }
}

void handleKeyboardEvent(SDL_Event event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
        currentColor[0] = 1.0f;
        currentColor[1] = 0.0f;
        currentColor[2] = 0.0f;
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_g) {
        currentColor[0] = 0.0f;
        currentColor[1] = 1.0f;
        currentColor[2] = 0.0f;
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_b) {
        currentColor[0] = 0.0f;
        currentColor[1] = 0.0f;
        currentColor[2] = 1.0f;
    }
}

int main(int argc, char** argv) {
    SDL_Window* window = NULL;
    SDL_GLContext context;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    window = SDL_CreateWindow("OpenGL Line Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    handleMouseEvent(event);
                    break;
                case SDL_KEYDOWN:
                    handleKeyboardEvent(event);
                    break;
                default:
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < lineCount; i++) {
            drawLine(lines[i]);
        }

        Point rectTopLeft = {100, 100};
        Point rectBottomRight = {150, 150};
        drawRect(rectTopLeft, rectBottomRight, currentColor);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}