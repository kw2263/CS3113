#include "Level1.h"

#define LEVEL1_OBJECT_COUNT 30
#define LEVEL1_ENEMY_COUNT 300

void Level1::Initialize() {

    state.nextScene = -1;
    state.win = false;

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.75f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 2.0f;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];

    GLuint jokerTextureID = Util::LoadTexture("joker.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].textureID = jokerTextureID;
        state.enemies[i].billboard = true;
        state.enemies[i].position = glm::vec3(rand() % 3 -1, 0.5, rand() % 20 - 200);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].velocity = glm::vec3(0, 0, rand() % 5 + 3);
    }

    state.objects = new Entity[LEVEL1_OBJECT_COUNT];

    GLuint floorTextureID = Util::LoadTexture("floor.jpg");

    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 10);

    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0, -0.25f, 0);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(4, 0.5f, 200);
    state.objects[0].entityType = FLOOR;

    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);

    state.objects[1].textureID = crateTextureID;
    state.objects[1].mesh = crateMesh;
    state.objects[1].position = glm::vec3(-1.25, 0.5, -5);
    state.objects[1].entityType = CRATE;

    state.objects[2].textureID = crateTextureID;
    state.objects[2].mesh = crateMesh;
    state.objects[2].position = glm::vec3(0, 0.5, -10);
    state.objects[2].entityType = CRATE;

    state.objects[3].textureID = crateTextureID;
    state.objects[3].mesh = crateMesh;
    state.objects[3].position = glm::vec3(1.25, 0.5, -10);
    state.objects[3].entityType = CRATE;

    state.objects[4].textureID = crateTextureID;
    state.objects[4].mesh = crateMesh;
    state.objects[4].position = glm::vec3(-1.25, 0.5, -15);
    state.objects[4].entityType = CRATE;

    state.objects[5].textureID = crateTextureID;
    state.objects[5].mesh = crateMesh;
    state.objects[5].position = glm::vec3(0, 0.5, -20);
    state.objects[5].entityType = CRATE;

    state.objects[6].textureID = crateTextureID;
    state.objects[6].mesh = crateMesh;
    state.objects[6].position = glm::vec3(1.25, 0.5, -20);
    state.objects[6].entityType = CRATE;

    state.objects[7].textureID = crateTextureID;
    state.objects[7].mesh = crateMesh;
    state.objects[7].position = glm::vec3(1.25, 0.5, -25);
    state.objects[7].entityType = CRATE;

    state.objects[8].textureID = crateTextureID;
    state.objects[8].mesh = crateMesh;
    state.objects[8].position = glm::vec3(0, 0.5, -25);
    state.objects[8].entityType = CRATE;

    state.objects[9].textureID = crateTextureID;
    state.objects[9].mesh = crateMesh;
    state.objects[9].position = glm::vec3(-1.25, 0.5, -30);
    state.objects[9].entityType = CRATE;

    state.objects[10].textureID = crateTextureID;
    state.objects[10].mesh = crateMesh;
    state.objects[10].position = glm::vec3(0, 0.5, -35);
    state.objects[10].entityType = CRATE;

    state.objects[11].textureID = crateTextureID;
    state.objects[11].mesh = crateMesh;
    state.objects[11].position = glm::vec3(-1.25, 0.5, -40);
    state.objects[11].entityType = CRATE;

    state.objects[12].textureID = crateTextureID;
    state.objects[12].mesh = crateMesh;
    state.objects[12].position = glm::vec3(1.25, 0.5, -40);
    state.objects[12].entityType = CRATE;

    state.objects[13].textureID = crateTextureID;
    state.objects[13].mesh = crateMesh;
    state.objects[13].position = glm::vec3(1.25, 0.5, -45);
    state.objects[13].entityType = CRATE;

    state.objects[14].textureID = crateTextureID;
    state.objects[14].mesh = crateMesh;
    state.objects[14].position = glm::vec3(0, 0.5, -50);
    state.objects[14].entityType = CRATE;

    state.objects[15].textureID = crateTextureID;
    state.objects[15].mesh = crateMesh;
    state.objects[15].position = glm::vec3(1.25, 0.5, -50);
    state.objects[15].entityType = CRATE;

    state.objects[16].textureID = crateTextureID;
    state.objects[16].mesh = crateMesh;
    state.objects[16].position = glm::vec3(1.25, 0.5, -55);
    state.objects[16].entityType = CRATE;

    state.objects[17].textureID = crateTextureID;
    state.objects[17].mesh = crateMesh;
    state.objects[17].position = glm::vec3(-1.25, 0.5, -60);
    state.objects[17].entityType = CRATE;

    state.objects[18].textureID = crateTextureID;
    state.objects[18].mesh = crateMesh;
    state.objects[18].position = glm::vec3(0, 0.5, -60);
    state.objects[18].entityType = CRATE;

    state.objects[19].textureID = crateTextureID;
    state.objects[19].mesh = crateMesh;
    state.objects[19].position = glm::vec3(-1.25, 0.5, -65);
    state.objects[19].entityType = CRATE;

    state.objects[20].textureID = crateTextureID;
    state.objects[20].mesh = crateMesh;
    state.objects[20].position = glm::vec3(0, 0.5, -65);
    state.objects[20].entityType = CRATE;

    state.objects[21].textureID = crateTextureID;
    state.objects[21].mesh = crateMesh;
    state.objects[21].position = glm::vec3(1.25, 0.5, -70);
    state.objects[21].entityType = CRATE;

    state.objects[22].textureID = crateTextureID;
    state.objects[22].mesh = crateMesh;
    state.objects[22].position = glm::vec3(-1.25, 0.5, -70);
    state.objects[22].entityType = CRATE;

    state.objects[23].textureID = crateTextureID;
    state.objects[23].mesh = crateMesh;
    state.objects[23].position = glm::vec3(0, 0.5, -75);
    state.objects[23].entityType = CRATE;

    state.objects[24].textureID = crateTextureID;
    state.objects[24].mesh = crateMesh;
    state.objects[24].position = glm::vec3(-1.25, 0.5, -75);
    state.objects[24].entityType = CRATE;

    state.objects[25].textureID = crateTextureID;
    state.objects[25].mesh = crateMesh;
    state.objects[25].position = glm::vec3(-1.25, 0.5, -80);
    state.objects[25].entityType = CRATE;

    state.objects[26].textureID = crateTextureID;
    state.objects[26].mesh = crateMesh;
    state.objects[26].position = glm::vec3(0, 0.5, -80);
    state.objects[26].entityType = CRATE;

    state.objects[27].textureID = crateTextureID;
    state.objects[27].mesh = crateMesh;
    state.objects[27].position = glm::vec3(-1.25, 0.5, -85);
    state.objects[27].entityType = CRATE;

    state.objects[28].textureID = crateTextureID;
    state.objects[28].mesh = crateMesh;
    state.objects[28].position = glm::vec3(1.25, 0.5, -90);
    state.objects[28].entityType = CRATE;

    state.objects[29].textureID = crateTextureID;
    state.objects[29].mesh = crateMesh;
    state.objects[29].position = glm::vec3(0, 0.5, -90);
    state.objects[29].entityType = CRATE;

}

void Level1::Update(float deltaTime) {

    if (!state.player->gameover) {
        state.player->Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT);

        for (int i = 0; i < LEVEL1_OBJECT_COUNT; i++) {
            state.objects[i].Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT);
        }

        for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
            state.enemies[i].Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT);
        }

        if (state.player->position.z < -94) {
            state.win = true;
        }
    }
}

void Level1::Render(ShaderProgram* program) {

    if (!state.player->gameover) {
        for (int i = 0; i < LEVEL1_OBJECT_COUNT; i++) {
            state.objects[i].Render(program);
        }

        for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
            state.enemies[i].Render(program);
        }

        if (state.win) {
            GLuint fontTextureID = Util::LoadTexture("pixel_font.png");
            Util::DrawText(program, fontTextureID, "YOU WIN", 1, -0.2f, glm::vec3(-2.5, 0, 0));
        }
        return;
    }

    GLuint fontTextureID = Util::LoadTexture("pixel_font.png");
    Util::DrawText(program, fontTextureID, "GAME OVER", 1, -0.2f, glm::vec3(-2.7, 0, 0));
}