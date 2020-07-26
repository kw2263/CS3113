#include "Scene.h"

class Menu : public Scene {
public:
	void Initialize(int lives) override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};