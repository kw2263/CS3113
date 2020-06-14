#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <stdlib.h>
#include <time.h>

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, rightMatrix, leftMatrix, ballMatrix, projectionMatrix;

glm::vec3 left_position = glm::vec3(-4.85f, 0, 0);
glm::vec3 left_movement = glm::vec3(0, 0, 0);
float left_speed = 5.0f;

glm::vec3 right_position = glm::vec3(4.85f, 0, 0);
glm::vec3 right_movement = glm::vec3(0, 0, 0);
float right_speed = 5.0f;

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);
float ball_speed = 3.0f;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("PONG!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    leftMatrix = glm::mat4(1.0f);
    rightMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

bool pressed = false;
float idk[] = { -2.0f, -1.5f, -1.0f, -0.5f, 0.5f, 1.0f, 1.5f, 2.0f };
float rand_x, rand_y;

void ProcessInput() {

    left_movement = glm::vec3(0);
    right_movement = glm::vec3(0);

    srand(time(NULL));
    rand_x = idk[rand() % 8];
    rand_y = idk[rand() % 8];

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                //
                break;

            case SDLK_RIGHT:
                //
                break;

            case SDLK_SPACE:
                //
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_SPACE] && !pressed) {
        ball_movement.x = rand_x;
        ball_movement.y = rand_y;
        pressed = true;
    }

    if (keys[SDL_SCANCODE_UP] && right_position.y <= 2.85f) {
        right_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN] && right_position.y >= -2.85f) {
        right_movement.y = -1.0f;
    }

    if (keys[SDL_SCANCODE_W] && left_position.y <= 2.85f) {
        left_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S] && left_position.y >= -2.85f) {
        left_movement.y = -1.0f;
    }

    if (glm::length(ball_movement) > 1.0f) {
        ball_movement = glm::normalize(ball_movement);
    }
}

float lastTicks = 0.0f;
bool gameon = true;

bool Colliding(glm::vec3 p1, glm::vec3 p2, float w1 = 0.2f, float w2 = 0.3f, float h1 = 0.2f, float h2 = 1.8f) {
    float xdist = fabs(p1.x - p2.x) - ((w1 + w2) / 2.0f);
    float ydist = fabs(p1.y - p2.y) - ((h1 + h2) / 2.0f);
    if (xdist < 0 && ydist < 0) {
        return true;
    }
    return false;
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (gameon) {
        left_position += left_movement * left_speed * deltaTime;
        leftMatrix = glm::mat4(1.0f);
        leftMatrix = glm::translate(leftMatrix, left_position);

        right_position += right_movement * right_speed * deltaTime;
        rightMatrix = glm::mat4(1.0f);
        rightMatrix = glm::translate(rightMatrix, right_position);


        if (ball_position.y >= 3.65f || ball_position.y <= -3.65f) {
            ball_movement.x = ball_movement.x;
            ball_movement.y = -ball_movement.y;
        }

        if (ball_position.x >= 4.9f || ball_position.x <= -4.9f) {
            gameon = false;
        }

        if (Colliding(ball_position, left_position) || Colliding(ball_position, right_position)) {
            ball_movement.x = -ball_movement.x;
            ball_movement.y = ball_movement.y;
        }

        ball_position += ball_movement * ball_speed * deltaTime;
        ballMatrix = glm::mat4(1.0f);
        ballMatrix = glm::translate(ballMatrix, ball_position);
    }
}

void Draw(glm::mat4 matrix) {
    program.SetModelMatrix(matrix);

    float vertices[] = {
        -0.15f, 0.9f, 0.15f, 0.9f, -0.15f, -0.9f,
        -0.15f, -0.9f, 0.15f, -0.9f, 0.15f, 0.9f
    };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
}

void DrawBall(glm::mat4 matrix) {
    program.SetModelMatrix(matrix);

    float vertices[] = {
        -0.1f, 0.1f, 0.1f, 0.1f, -0.1f, -0.1f,
        -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f
    };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Draw(leftMatrix);
    Draw(rightMatrix);
    DrawBall(ballMatrix);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}