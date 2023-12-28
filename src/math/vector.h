#pragma once

namespace electrodiux::math {

	struct vec3 {
		float x, y, z;

		vec3();
		vec3(float value);
		vec3(float x, float y, float z);

		vec3 operator+(const vec3& other) const {
			return { x + other.x, y + other.y, z + other.z };
		}

		vec3 operator-(const vec3& other) const {
			return { x - other.x, y - other.y, z - other.z };
		}

		vec3 operator*(const vec3& other) const {
			return { x * other.x, y * other.y, z * other.z };
		}

		vec3 operator/(const vec3& other) const {
			return { x / other.x, y / other.y, z / other.z };
		}

		vec3 operator*(float scalar) const {
			return { x * scalar, y * scalar, z * scalar };
		}

		vec3 operator/(float scalar) const {
			return { x / scalar, y / scalar, z / scalar };
		}

		vec3& operator+=(const vec3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		vec3& operator-=(const vec3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		vec3& operator*=(const vec3& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		vec3& operator/=(const vec3& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		vec3& operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		vec3& operator/=(float scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		bool operator==(const vec3& other) const {
			return x == other.x && y == other.y && z == other.z;
		}
	};

	vec3 operator*(float scalar, const vec3& v);

	vec3 operator/(float scalar, const vec3& v);

	float distance(const vec3& a, const vec3& b);

	float dot(const vec3& a, const vec3& b);

	vec3 cross(const vec3& a, const vec3& b);

	vec3 normalize(const vec3& v);

	vec3 lerp(const vec3& a, const vec3& b, float t);

	vec3 min(const vec3& v1, const vec3& v2);

	vec3 max(const vec3& v1, const vec3& v2);

}