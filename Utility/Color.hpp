#pragma once 
#include <cstdint>


struct ShakColor {
	constexpr ShakColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : b(b), g(g), r(r), a(a) {}
	ShakColor() = default;
	~ShakColor() = default;
	constexpr ShakColor(const ShakColor& c) : b(c.b), g(c.g), r(c.r), a(c.a) {}
	constexpr ShakColor(ShakColor&& c) noexcept : b(c.b), g(c.g), r(c.r), a(c.a) {}
	constexpr ShakColor& operator=(const ShakColor& c) {
		b = c.b;
		g = c.g;
		r = c.r;
		a = c.a;
		return *this;
	}
	uint8_t b, g, r, a;
};