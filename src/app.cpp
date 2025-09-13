#include "../include/app.hpp"

using Bloodlust::App;

App::App(const EngineConfig& config, int, char*[]) : config(config) {
	SDL_SetAppMetadata(config.appName, config.appVersion, config.appID);
}

SDL_AppResult App::changeScene(Scene* scene) {
	if (currentScene != nullptr) {
		SDL_AppResult exitResult = currentScene->onExit();
		if (exitResult != SDL_APP_CONTINUE) {
			return exitResult;
		}
	}
	currentScene = std::unique_ptr<Scene>(scene);
	return scene->onEnter();
}
