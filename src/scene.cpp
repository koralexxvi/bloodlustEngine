#include "../include/scene.hpp"

#include "../include/app.hpp"

using Bloodlust::Scene;

Bloodlust::AppResult Scene::onEnter() {
	return AppResult::CONTINUE;
}

Bloodlust::AppResult Scene::onExit() {
	return AppResult::CONTINUE;
}
