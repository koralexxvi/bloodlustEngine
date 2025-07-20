#include "../include/app.hpp"

using Bloodlust::App;

Bloodlust::AppResult App::changeScene(Scene* scene) {
	if (currentScene != nullptr) {
		AppResult exitResult = currentScene->onExit();
		if (exitResult != AppResult::CONTINUE) {
			return exitResult;
		}
	}
	currentScene = std::unique_ptr<Scene>(scene);
	return scene->onEnter();
}
