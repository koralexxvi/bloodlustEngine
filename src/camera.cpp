#include "../include/camera.hpp"

#include "../include/app.hpp"
#include "../include/object/collider.hpp"

#include <SDL3/SDL_assert.h>
#include <glm/gtc/matrix_transform.hpp>

using Bloodlust::Camera;

Camera::Camera(const Vector2& pos, const TargetSettings& settings, float fov) noexcept
	: pos(pos + app->window->getSize().toVector() * 0.5f),
	  fov(fov) {
	setTargetSettings(settings);
}

glm::mat4 Camera::getMatrix() const noexcept {
	Vector2 res = app->window->getSize().toVector();
	Vector2 translation = pos + offset;
	glm::mat4 projection = glm::ortho(0.0f, fov * (res.x / res.y), 0.0f, fov);
	projection = glm::translate(projection, glm::vec3(translation.x, translation.y, 0.0f));
	return projection;
}

void Camera::step(std::uint64_t dt) noexcept {
	if (targetSettings.catchupSpeed == 0.0f) {
		return;
	}
	Vector2 halfRes = app->window->getSize().toVector() * 0.5f;
	Vector2 targetPoint;
	switch (targetSettings.targetMode) {
		case TargetMode::FOLLOW_POINT: targetPoint = posTarget; break;
		case TargetMode::FOLLOW_COLLIDER: {
			SDL_assert(colliderTarget != nullptr);
			targetPoint = colliderTarget->position;
			break;
		}
		default: return;
	}
	targetPoint += halfRes;
	bool moved = false;
	Vector2 halfDeadzone = targetSettings.deadzone * 0.5f;
	Vector2 diff = targetPoint - pos;
	float fdt = static_cast<float>(
		static_cast<double>(dt) / static_cast<double>(app->getConfig().appTimestep)
	);
	followSpeed += targetSettings.catchupSpeed * fdt;
	if (SDL_fabsf(diff.x) > halfDeadzone.x) {
		pos.x -= SDL_copysignf(halfDeadzone.x, diff.x) - diff.x;
		moved = true;
	}
	if (SDL_fabsf(diff.y) > halfDeadzone.y) {
		pos.y -= SDL_copysignf(halfDeadzone.y, diff.y) - diff.y;
		moved = true;
	}
	if (!moved) {
		followSpeed = 0.0f;
	}
	if (boundsActive) {
		pos = {
			std::clamp(pos.x, bounds.x - halfRes.x, (bounds.x + bounds.w) - halfRes.x),
			std::clamp(pos.y, bounds.y - halfRes.y, (bounds.y + bounds.h) - halfRes.y)
		};
	}
}
