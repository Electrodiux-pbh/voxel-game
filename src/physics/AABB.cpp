#include "AABB.h"

namespace electrodiux::phy {

	AABB::AABB() : min(math::vec3()), max(math::vec3()) {}

	AABB::AABB(const math::vec3& min, const math::vec3& max) : min(min), max(max) {}

	void AABB::setMin(const math::vec3& min) {
		this->min = min;
	}

	void AABB::setMax(const math::vec3& max) {
		this->max = max;
	}

	math::vec3 AABB::getMin() const {
		return min;
	}

	math::vec3 AABB::getMax() const {
		return max;
	}

	bool AABB::intersects(const AABB& other) const {
		return min.x <= other.max.x && max.x >= other.min.x &&
			min.y <= other.max.y && max.y >= other.min.y &&
			min.z <= other.max.z && max.z >= other.min.z;
	}

	bool AABB::contains(const math::vec3& point) const {
		return point.x >= min.x && point.x <= max.x &&
			point.y >= min.y && point.y <= max.y &&
			point.z >= min.z && point.z <= max.z;
	}

	bool AABB::operator==(const AABB& other) const {
		return min == other.min && max == other.max;
	}

	math::vec3 AABB::getCenter() const {
		return (min + max) / 2;
	}

	math::vec3 AABB::getExtents() const {
		return max - min;
	}

	float AABB::getVolume() const {
		math::vec3 extents = getExtents();
		return extents.x * extents.y * extents.z;
	}

	bool AABB::isInside(const AABB& other) const {
		return min.x >= other.min.x && max.x <= other.max.x &&
			min.y >= other.min.y && max.y <= other.max.y &&
			min.z >= other.min.z && max.z <= other.max.z;
	}

}