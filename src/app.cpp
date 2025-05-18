#include "../include/app.hpp"

using Bloodlust::App;

bool App::changeScene(Scene* scene) {
	if (currentScene != nullptr) {
		if (!currentScene->onExit()) {
			return false;
		}
	}
	currentScene = std::unique_ptr<Scene>(scene);
	return scene->onEnter();
}
