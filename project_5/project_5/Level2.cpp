#include "Level2.h"

#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1

#define TEXT_COUNT 3

unsigned int level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 1, 1, 1, 1, 1, 0, 0, 3, 3, 3, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3
};

void Level2::Initialize(int lives) {

    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);

    // Move over all of the player and enemy code from initialization.
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 5.8f;
    state.player->lives = lives;

    state.texts = new Entity[TEXT_COUNT];
    GLuint fontTextureID = Util::LoadTexture("pixel_font.png");

    state.texts[0].textureID = fontTextureID;
    state.texts[0].msg = "You Lose";
    state.texts[0].isFont = true;
    state.texts[0].fontPosition = glm::vec3(6, -2.5, 0);
    state.texts[0].fontSize = 0.5f;
    state.texts[0].isActive = false;

    state.texts[1].textureID = fontTextureID;
    state.texts[1].msg = "You Win";
    state.texts[1].isFont = true;
    state.texts[1].fontPosition = glm::vec3(3, -2.5, 0);
    state.texts[1].fontSize = 0.5f;
    state.texts[1].isActive = false;

    state.texts[2].textureID = fontTextureID;
    state.texts[2].msg = "lives: " + std::to_string(state.player->lives);
    state.texts[2].isFont = true;
    state.texts[2].fontPosition = glm::vec3(1.5, -1, 0);
    state.texts[2].fontSize = 0.3f;

    state.enemies = new Entity();
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies->textureID = enemyTextureID;
    state.enemies->entityType = ENEMY;
    state.enemies->position = glm::vec3(10, 0, 0);
    state.enemies->speed = 1.5f;
    state.enemies->jumpPower = 5.8f;
    state.enemies->acceleration = glm::vec3(0, -5.0f, 0);
    state.enemies->aiType = JUMPER;
    state.enemies->aiState = JUMPING;
}

void Level2::Update(float deltaTime) {

    if (state.player->lives == 0) {
        state.texts[0].isActive = true;
        return;
    }
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    state.enemies->Update(deltaTime, state.player, NULL, 0, state.map);

    if (state.player->position.x >= 14) {
        state.nextScene = 3;
    }

    else if (state.player->position.y < -8 || state.player->isActive == false) {
        (state.player->lives)--;
        state.nextScene = 2;
    }
}

void Level2::Render(ShaderProgram* program) {
    for (int i = 0; i < TEXT_COUNT; i++) {
        state.texts[i].Render(program);
    }
    state.map->Render(program);
    state.player->Render(program);
    state.enemies->Render(program);
}