#pragma once

namespace electrodiux::voxel::gfx {
	
	class Color {

	private:
		float _r, _g, _b, _a;

	public:
		Color(float r, float g, float b, float a) : _r(r), _g(g), _b(b), _a(a) {}
		Color(float r, float g, float b) : Color(r, g, b, 1) {}
		Color() : Color(0, 0, 0, 1) {}

		float r() const;
		float g() const;
		float b() const;
		float a() const;

		void setR(float r);
		void setG(float g);
		void setB(float b);
		void setA(float a);

		void set(float r, float g, float b, float a);

		static Color& lerp(const Color& color1, const Color& color2, float t, Color& out);

		bool operator==(const Color& other) const;
	};

}