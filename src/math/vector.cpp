#include "vector.h"

#include <cmath>

namespace electrodiux::math {

	vec3::vec3() : x(0), y(0), z(0) {}

	vec3::vec3(float value) : x(value), y(value), z(value) {}

	vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	vec3 operator*(float scalar, const vec3& v) {
		return { scalar * v.x, scalar * v.y, scalar * v.z };
	}

	vec3 operator/(float scalar, const vec3& v) {
		return { scalar / v.x, scalar / v.y, scalar / v.z };
	}

	float distance(const vec3& a, const vec3& b) {
		return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));
	}

	float dot(const vec3& a, const vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	vec3 cross(const vec3& a, const vec3& b) {
		return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
	}

	vec3 normalize(const vec3& v) {
		float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		return { v.x / length, v.y / length, v.z / length };
	}

	vec3 lerp(const vec3& a, const vec3& b, float t) {
		return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t };
	}

	vec3 min(const vec3& v1, const vec3& v2) {
		return { std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z) };
	}

	vec3 max(const vec3& v1, const vec3& v2) {
		return { std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z) };
	}

}