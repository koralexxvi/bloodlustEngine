#include <bloodlustEngine/scene.hpp>

using Bloodlust::Scene;

SDL_AppResult Scene::step() noexcept {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::draw(std::uint64_t) noexcept {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::onEnter() noexcept {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::onExit() noexcept {
	return SDL_APP_CONTINUE;
}

SDL_AppResult Scene::onInput(const InputEvent&) noexcept {
	return SDL_APP_CONTINUE;
}
