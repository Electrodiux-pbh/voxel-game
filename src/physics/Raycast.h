#pragma once

#include "../math/vector.h"
#include "AABB.h"

namespace electrodiux::phy {

	class Raycast {

	private:
	math::vec3 origin;
	math::vec3 direction;

	public:
	Raycast(const math::vec3& origin, const math::vec3& direction);

	void setOrigin(const math::vec3& origin);
	void setDirection(const math::vec3& direction);

	math::vec3 getOrigin() const;
	math::vec3 getDirection() const;

	};

	Raycast raycastFromAnglesDeg(const math::vec3& origin, float x_angle, float y_angle);
	Raycast raycastFromAnglesRad(const math::vec3& origin, float x_angle, float y_angle);

	bool intersects(const Raycast& ray, const AABB& aabb);
	AABBface intersectsFace(const Raycast& ray, const AABB& aabb);

}