#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "SDL_mixer.h"

#include "Util.h"
#include "Entity.h"
#include "Scene.h"
#include "Level1.h"
#include "Menu.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool on = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, uiViewMatrix, uiProjectionMatrix;

Scene* currentScene;
Scene* sceneList[2];

Mix_Music* music;
Mix_Chunk* ah;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("bgm.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    ah = Mix_LoadWAV("ah.wav");

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    sceneList[1] = new Level1();
    sceneList[0] = new Menu();
    SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        if (currentScene == sceneList[0]) {
                            currentScene->state.nextScene = 1;
                        }
                        
                }
                break;
        }
    }

    if (currentScene != sceneList[0]) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        currentScene->state.player->velocity.x = 0;
        currentScene->state.player->velocity.z = 0;


        if (keys[SDL_SCANCODE_A]) {
            if (currentScene->state.player->position.x > -1.5f) {
                currentScene->state.player->velocity.x = -1.5f;
            }
            //state.player->rotation.y += 0.3f;
        }
        else if (keys[SDL_SCANCODE_D]) {
            if (currentScene->state.player->position.x < 1.5f) {
                currentScene->state.player->velocity.x = 1.5f;
            }
            //state.player->rotation.y -= 0.3f;
        }
        else if (keys[SDL_SCANCODE_W]) {
            currentScene->state.player->velocity.z = -1.3f;

        }
        else if (keys[SDL_SCANCODE_S]) {
            currentScene->state.player->velocity.z = 1.0f;

        }

    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    if (currentScene != sceneList[0]) {
        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) {
            accumulator = deltaTime;
            return;
        }

        while (deltaTime >= FIXED_TIMESTEP) {
            currentScene->Update(FIXED_TIMESTEP);

            deltaTime -= FIXED_TIMESTEP;
        }

        accumulator = deltaTime;

        viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::rotate(viewMatrix,
            glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
        viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    if (currentScene == sceneList[0] || currentScene->state.win || currentScene->state.player->gameover) {
        program.SetProjectionMatrix(uiProjectionMatrix);
        program.SetViewMatrix(uiViewMatrix);

        if (currentScene != sceneList[0]) {
            Mix_HaltMusic();
            if (currentScene->state.player->gameover) Mix_PlayChannel(-1, ah, 0);
            on = false;
        }
    }
    
    currentScene->Render(&program);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        if (on) {
            ProcessInput();
            Update();

            if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
            Render();
        }
    }
    
    Shutdown();
    return 0;
}
