#pragma once

#include "datatype/vector2.hpp"
#include "object/collider.hpp"

#include <glm/mat4x4.hpp>

namespace Bloodlust {

class Camera {
  public:
	enum class TargetMode : int32_t {
		FIXED,
		FOLLOW_POINT,
		FOLLOW_COLLIDER
	};

	struct TargetSettings {
		TargetMode targetMode;
		float minDistance;
		float maxDistance;
		float catchupSpeed;
	};

  private:
	TargetSettings targetSettings;
	const Collider* colliderTarget = nullptr;
	Vector2 pos = {};
	Vector2 posTarget = {};
	Vector2 offset = {};
	float fov = 256.0f;

	[[maybe_unused]]
	char _padding[4];

  public:
	Camera(const Vector2& pos, const TargetSettings& settings, float fov);

	Vector2 getPosition() const;
	void setPosition(const Vector2& newPos);

	Vector2 getOffset() const;
	void setOffset(const Vector2& newOffset);

	float getFOV() const;
	void setFOV(float newFOV);

	TargetSettings getTargetSettings() const;
	void setTargetSettings(const TargetSettings& newSettings);

	void setTarget(const Collider* collider);
	void setTarget(const Vector2& point);

	glm::mat4 getMatrix() const;

	void step(uint64_t dt);
};

}  // namespace Bloodlust
