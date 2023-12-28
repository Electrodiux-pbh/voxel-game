#pragma once

#include "../math/vector.h"

namespace electrodiux::phy {

	class AABB {

		private:
		math::vec3 min;
		math::vec3 max;

		public:
		AABB();

		AABB(const math::vec3& min, const math::vec3& max);

		void setMin(const math::vec3& min);
		void setMax(const math::vec3& max);

		math::vec3 getMin() const;
		math::vec3 getMax() const;

		bool intersects(const AABB& other) const;

		bool contains(const math::vec3& point) const;

		bool operator==(const AABB& other) const;

		math::vec3 getCenter() const;

		math::vec3 getExtents() const;

		float getVolume() const;

		bool isInside(const AABB& other) const;

	};

	typedef char AABBface;

	constexpr char FACE_BOTTOM = 0b00000001;
	constexpr char FACE_TOP = 0b00000010;
	constexpr char FACE_LEFT = 0b00000100;
	constexpr char FACE_RIGHT = 0b00001000;
	constexpr char FACE_FRONT = 0b00010000;
	constexpr char FACE_BACK = 0b00100000;

}
