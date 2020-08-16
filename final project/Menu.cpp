#include "Menu.h"

void Menu::Initialize() {

    state.nextScene = -1;

}

void Menu::Update(float deltaTime) {
    return;
}

void Menu::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("pixel_font.png");
    Util::DrawText(program, fontTextureID, "RUN", 2, -0.2f, glm::vec3(-1.7, 0, 0));
    Util::DrawText(program, fontTextureID, "press ENTER to start", 0.5, -0.1f, glm::vec3(-3.5, -2.0f, 0));
}