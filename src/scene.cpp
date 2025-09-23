#include "../include/scene.hpp"

using Bloodlust::Scene;

SDL_AppResult Scene::step() {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::draw(std::uint64_t) {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::onEnter() {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::onExit() {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::onInput(const InputEvent&) {
	return SDL_APP_CONTINUE;
}
