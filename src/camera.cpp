#include "../include/camera.hpp"

#include "../include/app.hpp"
#include "../include/config.hpp"
#include "../include/render/window.hpp"

#include <SDL3/SDL_assert.h>
#include <glm/gtc/matrix_transform.hpp>

using Bloodlust::Camera;

Camera::Camera(const Vector2& pos, const TargetSettings& settings, float fov)
	: targetSettings(settings),
	  pos(pos),
	  fov(fov) {}

void Camera::setPosition(const Vector2& newPos) {
	pos = newPos;
}

void Camera::setOffset(const Vector2& newOffset) {
	offset = newOffset;
}

void Camera::setFOV(float newFOV) {
	SDL_assert(newFOV > 0.0f);
	fov = newFOV;
}

void Camera::setTarget(const Collider* collider) {
	colliderTarget = collider;
}

void Camera::setTarget(const Vector2& point) {
	posTarget = point;
}

void Camera::setTargetSettings(const TargetSettings& newSettings) {
	targetSettings = newSettings;
}

glm::mat4 Camera::getMatrix() const {
	Vector2 res = app->window->getSize();
	Vector2 translation = pos + offset;
	glm::mat4 projection = glm::ortho(0.0f, fov * (res.x / res.y), 0.0f, fov);
	projection = glm::translate(projection, glm::vec3(translation.x, translation.y, 0.0f));
	return projection;
}

void Camera::step(std::uint64_t dt) {
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
	if (targetSettings.catchupSpeed <= 0.0f) {
		pos = targetPoint;
		return;
	}
	Vector2 diff = pos - targetPoint;
	float distance = diff.magnitude() - targetSettings.minDistance;
	if (distance < 0.0f) {
		return;
	}
	targetPoint += diff;
	float alpha =
		static_cast<float>(static_cast<double>(dt) / static_cast<double>(config.appTimestep))
		* targetSettings.catchupSpeed;
	if (alpha > 1.0f) {
		alpha = 1.0f;
	}
	Vector2 newPos = pos.lerp(targetPoint, alpha);
	if (pos.fuzzyEq(newPos, 0.01f)) {
		pos = targetPoint;
	} else {
		pos = newPos;
	}
}
