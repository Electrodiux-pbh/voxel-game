#include "Raycast.h"

#include <glm/glm.hpp>

namespace electrodiux::phy {

    Raycast::Raycast(const math::vec3& origin, const math::vec3& direction) {
		this->origin = origin;
		this->direction = direction;
	}

	void Raycast::setOrigin(const math::vec3& origin) {
		this->origin = origin;
	}

	void Raycast::setDirection(const math::vec3& direction) {
		this->direction = direction;
	}

	math::vec3 Raycast::getOrigin() const {
		return origin;
	}

	math::vec3 Raycast::getDirection() const {
		return direction;
	}

	Raycast raycastFromAnglesDeg(const math::vec3& origin, float x_angle, float y_angle) {
		return raycastFromAnglesRad(origin, glm::radians(x_angle), glm::radians(y_angle));
	}

	Raycast raycastFromAnglesRad(const math::vec3& origin, float x_angle, float y_angle) {
		float cosX = std::cos(x_angle);
		float sinX = std::sin(x_angle);
		float cosY = std::cos(y_angle);
		float sinY = std::sin(y_angle);

		math::vec3 direction(
			cosX * sinY,
			-sinX,
			-cosY * cosX
		);

		return Raycast(origin, direction);
	}

	bool intersects(const Raycast& ray, const AABB& aabb) {
		math::vec3 ray_direction = ray.getDirection();

		// Calculate the inverse of the ray's direction
		math::vec3 inverse_dir = 1.0f / ray_direction;

		// Calculate t-values for each pair of planes
		math::vec3 tMin = (aabb.getMin() - ray.getOrigin()) * inverse_dir;
		math::vec3 tMax = (aabb.getMax() - ray.getOrigin()) * inverse_dir;

		// Find the maximum and minimum t-values for each axis
		math::vec3 tEnter = math::min(tMin, tMax);
		math::vec3 tExit = math::max(tMin, tMax);

		// Find the maximum of the minimum t-values (entry) and the minimum of the maximum t-values (exit)
		float _tEntry = glm::max(glm::max(tEnter.x, tEnter.y), tEnter.z);
		float _tExit = glm::min(glm::min(tExit.x, tExit.y), tExit.z);

		return (_tEntry < _tExit) && (_tExit > 0.0f);
	}

	AABBface intersectsFace(const Raycast& ray, const AABB& aabb) {
		math::vec3 ray_direction = ray.getDirection();

		// Calculate the inverse of the ray's direction
		math::vec3 inverse_dir = 1.0f / ray_direction;

		// Calculate t-values for each pair of planes
		math::vec3 tMin = (aabb.getMin() - ray.getOrigin()) * inverse_dir;
		math::vec3 tMax = (aabb.getMax() - ray.getOrigin()) * inverse_dir;

		// Find the maximum and minimum t-values for each axis
		math::vec3 tEnter = math::min(tMin, tMax);
		math::vec3 tExit = math::max(tMin, tMax);

		// Find the maximum of the minimum t-values (entry) and the minimum of the maximum t-values (exit)
		float _tEntry = glm::max(glm::max(tEnter.x, tEnter.y), tEnter.z);
		float _tExit = glm::min(glm::min(tExit.x, tExit.y), tExit.z);

		if ((_tEntry < _tExit) && (_tExit > 0.0f)) {
			// Calculate the normal vector of the hit face
			AABBface face = 0;

			if (tEnter.x > tEnter.y && tEnter.x > tEnter.z) {
				face |= (ray_direction.x > 0.0f) ? FACE_LEFT : FACE_RIGHT;
			} else if (tEnter.y > tEnter.x && tEnter.y > tEnter.z) {
				face |= (ray_direction.y > 0.0f) ? FACE_BOTTOM : FACE_TOP;
			} else {
				face |= (ray_direction.z > 0.0f) ? FACE_FRONT : FACE_BACK;
			}

			return face;
		}

		return AABBface(0);
	}

}
