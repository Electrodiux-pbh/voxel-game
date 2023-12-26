#include "Color.h"

namespace electrodiux::voxel::gfx {

	float Color::r() const {
		return this->_r;
	}

	float Color::g() const {
		return this->_g;
	}

	float Color::b() const {
		return this->_b;
	}

	float Color::a() const {
		return this->_a;
	}

	void Color::setR(float r) {
		this->_r = r;
	}

	void Color::setG(float g) {
		this->_g = g;
	}

	void Color::setB(float b) {
		this->_b = b;
	}

	void Color::setA(float a) {
		this->_a = a;
	}

	void Color::set(float r, float g, float b, float a) {
		this->_r = r;
		this->_g = g;
		this->_b = b;
		this->_a = a;
	}

	inline float lerpf(float a, float b, float t) {
		return a + (b - a) * t;
	}

	inline float clamp(float x, float a, float b) {
		return x < a ? a : (x > b ? b : x);
	}

	Color& Color::lerp(const Color& color1, const Color& color2, float t, Color& out) {
		t = clamp(t, 0.0f, 1.0f);

		out.setA(lerpf(color1.a(), color2.a(), t));
		out.setR(lerpf(color1.r(), color2.r(), t));
		out.setG(lerpf(color1.g(), color2.g(), t));
		out.setB(lerpf(color1.b(), color2.b(), t));
		
		return out;
	}

	bool Color::operator==(const Color& other) const {
		return _r == other._r && _g == other._g && _b == other._b && _a == other._a;
	}
}