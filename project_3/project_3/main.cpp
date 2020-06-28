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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 34

struct GameState {
    bool gameon = true;
    Entity* platforms;
    Entity* ship;
    Entity* failMsg;
    Entity* successMsg;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    // Initialize Game Objects

    state.platforms = new Entity[PLATFORM_COUNT];

    GLuint lavaTextureID = LoadTexture("platformPack_tile006.png");
    GLuint landingTextureID = LoadTexture("platformPack_tile002.png");
    GLuint wallTextureID = LoadTexture("platformPack_tile020.png");
    GLuint shipTextureID = LoadTexture("playerShip3_blue.png");
    GLuint fontTextureID = LoadTexture("pixel_font.png");

    state.failMsg = new Entity();
    state.successMsg = new Entity();

    state.failMsg->isActive = false;
    state.failMsg->textureID = fontTextureID;
    state.failMsg->msg = "MISSION FAILED";
    state.failMsg->isFont = true;

    state.successMsg->isActive = false;
    state.successMsg->textureID = fontTextureID;
    state.successMsg->msg = "MISSION SUCCESS";
    state.successMsg->isFont = true;

    state.platforms[0].textureID = landingTextureID;
    state.platforms[0].position = glm::vec3(-1, -3.25f, 0);
    state.platforms[0].type = SAVE;

    state.platforms[1].textureID = landingTextureID;
    state.platforms[1].position = glm::vec3(-2, -3.25f, 0);
    state.platforms[1].type = SAVE;

    state.platforms[2].textureID = lavaTextureID;
    state.platforms[2].position = glm::vec3(-4, -3.25f, 0);
    state.platforms[2].type = DANGER;

    state.platforms[3].textureID = lavaTextureID;
    state.platforms[3].position = glm::vec3(-3, -3.25f, 0);
    state.platforms[3].type = DANGER;

    for (int i = 0; i < 5; i++) {
        state.platforms[i + 4].textureID = lavaTextureID;
        state.platforms[i + 4].position = glm::vec3(i, -3.25f, 0);
        state.platforms[i + 4].type = DANGER;
    }

    int index = 9;
    
    for (float i = -3.25f; i < 4.0f; i++) {
        state.platforms[index].textureID = wallTextureID;
        state.platforms[index].position = glm::vec3(-5, i, 0);
        state.platforms[index].type = DANGER;
        index++;
        state.platforms[index].textureID = wallTextureID;
        state.platforms[index].position = glm::vec3(5, i, 0);
        state.platforms[index].type = DANGER;
        index++;
    }

    for (int i = -4; i < 5; i++) {
        if (i < -1 || i > 1) {
            state.platforms[index].textureID = wallTextureID;
            state.platforms[index].position = glm::vec3(i, -0.25f, 0);
            state.platforms[index].type = DANGER;
            index++;
        }
    }

    for (int i = -4; i < 5; i++) {
        if (i == -4 || i == -1 || i == 4) {
            state.platforms[index].textureID = wallTextureID;
            state.platforms[index].position = glm::vec3(i, 1.75f, 0);
            state.platforms[index].type = DANGER;
            index++;
        }
    }

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0);
    }
    
    state.ship = new Entity();
    state.ship->type = SAVE;
    state.ship->position = glm::vec3(-1, 3.75f, 0);
    state.ship->movement = glm::vec3(0);
    state.ship->acceleration = glm::vec3(0, -0.6f, 0);
    state.ship->speed = 1.5f;
    state.ship->textureID = shipTextureID;
    state.ship->height = 1.0f;
    state.ship->width = 1.0f;
}

void ProcessInput() {
    
    state.ship->movement = glm::vec3(0);
    
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
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        //jump
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.ship->movement.x = -2.0f;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.ship->movement.x = 2.0f;
    }

    if (glm::length(state.ship->movement) > 1.0f) {
        state.ship->movement = glm::normalize(state.ship->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (state.gameon) {
        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }

        while (deltaTime >= FIXED_TIMESTEP) {
            state.ship->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);

            if (state.ship->lastCollision == SAVE) {
                state.successMsg->isActive = true;
                state.gameon = false;
                return;
            }

            if (state.ship->collidedTop || state.ship->collidedBottom ||
                state.ship->collidedLeft || state.ship->collidedRight) {

                state.failMsg->isActive = true;
                state.gameon = false;
                return;
            }

            deltaTime -= FIXED_TIMESTEP;
        }

        accumulator = deltaTime;
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    
    state.ship->Render(&program);

    state.failMsg->Render(&program);
    state.successMsg->Render(&program);
    
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
