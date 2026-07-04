#pragma once

#include "datatype/vector2.hpp"
#include "object/collider.hpp"

#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_rect.h>
#include <glm/mat4x4.hpp>

namespace Bloodlust {

// A camera that controls the world's visible viewport.
class Camera {
  public:
	// The follow modes that a camera can use.
	enum class TargetMode : std::int32_t {
		FIXED,			 // The camera does not move.
		FOLLOW_POINT,	 // The camera will follow a specified vector.
		FOLLOW_COLLIDER	 // The camera will follow the center of a collider.
	};

	// A camera's target settings.
	struct TargetSettings {
		// The follow mode this camera will use.
		TargetMode targetMode;

		// The deadzone of the camera. If set to <0, 0>, then the deadzone is disabled and the
		// camera will always center onto the target.
		Vector2 deadzone;

		// The speed at which a camera will move to follow its target.
		float catchupSpeed;
	};

  private:
	// The camera's target settings.
	TargetSettings targetSettings;

	// If the target mode is set to FOLLOW_COLLIDER, the collider that this camera will follow,
	// otherwise ignored.
	const Collider* colliderTarget = nullptr;

	// The maximum boundaries that the camera's position will be clamped to.
	SDL_FRect bounds = {};

	// The current position of the camera.
	Vector2 pos = {};

	// If the target mode is set to FOLLOW_POINT, the point that this camera will follow towards,
	// otherwise ignored.
	Vector2 posTarget = {};

	// An offset that will be applied to the camera's position.
	Vector2 offset = {};

	// The vertical field of view that this camera will be able to show.
	float fov = 256.0f;

	// The current speed that the camera is moving at.
	float followSpeed = 0.0f;

	// Whether or not this camera will clamp its position to its boundaries.
	bool boundsActive = false;

	[[maybe_unused]]
	char _padding[7];

  public:
	// Constructs a new camera at the specified position with the given settings and vertical FOV.
	Camera(const Vector2& pos, const TargetSettings& settings, float fov) noexcept;

	// Returns whether or not the camera boundaries are currently active.
	constexpr bool getBoundsActive() const noexcept {
		return boundsActive;
	}

	// Returns the current boundaries of the camera.
	constexpr SDL_FRect getBounds() const noexcept {
		return bounds;
	}

	// Sets the maximum boundaries that this camera's position will be clamped to.
	inline void setBounds(const SDL_FRect& newBounds) noexcept {
		bounds = newBounds;
		boundsActive = true;
	}

	// Clears the boundaries of this camera.
	inline void clearBounds() noexcept {
		boundsActive = false;
	}

	// Returns the current position of the camera.
	constexpr Vector2 getPosition() const noexcept {
		return pos;
	}

	// Sets the position of the camera.
	inline void setPosition(const Vector2& newPos) noexcept {
		pos = newPos;
	}

	// Returns the current camera offset.
	constexpr Vector2 getOffset() const noexcept {
		return offset;
	}

	// Sets the current camera offset.
	inline void setOffset(const Vector2& newOffset) noexcept {
		offset = newOffset;
	}

	// Returns the current vertical FOV of the camera.
	constexpr float getFOV() const noexcept {
		return fov;
	}

	// Sets the vertical FOV of the camera.
	inline void setFOV(float newFOV) noexcept {
		SDL_assert(newFOV > 0.0f);
		fov = newFOV;
	}

	// Returns a const reference to this camera's target settings.
	constexpr const TargetSettings& getTargetSettings() const noexcept {
		return targetSettings;
	}

	// Sets the target settings of this camera.
	inline void setTargetSettings(const TargetSettings& newSettings) noexcept {
		SDL_assert(newSettings.deadzone.x >= 0.0f && newSettings.deadzone.y >= 0.0f);
		targetSettings = newSettings;
	}

	// Sets the target of this camera to a collider.
	inline void setTarget(const Collider* collider) noexcept {
		colliderTarget = collider;
	}

	// Sets the target of this camera to the given point.
	inline void setTarget(const Vector2& point) noexcept {
		posTarget = point;
	}

	// Returns the camera's current matrix transform.
	glm::mat4 getMatrix() const noexcept;

	// Steps the camera forward in time.
	void step(std::uint64_t dt) noexcept;
};

}  // namespace Bloodlust
