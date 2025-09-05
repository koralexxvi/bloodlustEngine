#pragma once

#include "datatype/vector2.hpp"
#include "object/collider.hpp"

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

		// The minimum distance a camera can be behind a target.
		float minDistance;

		// The maximum distance a camera can lag behind a target.
		float maxDistance;

		// The speed at which a camera will move to follow its target.
		float catchupSpeed;
	};

  private:
	// The camera's target settings.
	TargetSettings targetSettings;

	// If the target mode is set to FOLLOW_COLLIDER, the collider that this camera will follow,
	// otherwise ignored.
	const Collider* colliderTarget = nullptr;

	// The current position of the camera.
	Vector2 pos = {};

	// If the target mode is set to FOLLOW_POINT, the point that this camera will follow towards,
	// otherwise ignored.
	Vector2 posTarget = {};

	// An offset that will be applied to the camera's position.
	Vector2 offset = {};

	// The vertical field of view that this camera will be able to show.
	float fov = 256.0f;

	[[maybe_unused]]
	char _padding[4];

  public:
	// Constructs a new camera at the specified position with the given settings and vertical FOV.
	Camera(const Vector2& pos, const TargetSettings& settings, float fov);

	// Returns the current position of the camera.
	Vector2 getPosition() const;

	// Sets the position of the camera.
	void setPosition(const Vector2& newPos);

	// Returns the current camera offset.
	Vector2 getOffset() const;

	// Sets the current camera offset.
	void setOffset(const Vector2& newOffset);

	// Returns the current vertical FOV of the camera.
	float getFOV() const;

	// Sets the vertical FOV of the camera.
	void setFOV(float newFOV);

	// Returns a copy of this camera's target settings.
	TargetSettings getTargetSettings() const;

	// Sets the target settings of this camera.
	void setTargetSettings(const TargetSettings& newSettings);

	// Sets the target of this camera to a collider.
	void setTarget(const Collider* collider);

	// Sets the target of this camera to the given point.
	void setTarget(const Vector2& point);

	// Returns the camera's current matrix transform.
	glm::mat4 getMatrix() const;

	// Steps the camera forward in time.
	void step(std::uint64_t dt);
};

}  // namespace Bloodlust
