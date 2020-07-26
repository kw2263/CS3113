#include "Menu.h"

#define TEXT_COUNT 2

void Menu::Initialize() {

    state.nextScene = -1;

    state.texts = new Entity[TEXT_COUNT];
    GLuint fontTextureID = Util::LoadTexture("pixel_font.png");

    state.texts[0].textureID = fontTextureID;
    state.texts[0].msg = "3 Level Game";
    state.texts[0].isFont = true;
    state.texts[0].fontPosition = glm::vec3(-2.75f, 1, 0);

    state.texts[1].textureID = fontTextureID;
    state.texts[1].msg = "press Enter to start";
    state.texts[1].isFont = true;
    state.texts[1].fontPosition = glm::vec3(-4.75f, -1, 0);

}

void Menu::Update(float deltaTime) {
    return;
}

void Menu::Render(ShaderProgram* program) {

    for (int i = 0; i < TEXT_COUNT; i++) {
        state.texts[i].Render(program);
    }
}