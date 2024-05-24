#pragma once


class vec2 {
public:
	float x, y;
	constexpr inline vec2() {}
	constexpr inline vec2(float x, float y) : x(x), y(y) {}
	constexpr inline vec2(const vec2& v) : x(v.x), y(v.y) {}
	constexpr inline vec2(vec2&& v) noexcept : x(v.x), y(v.y) {}
	constexpr inline vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}


	// operators
	constexpr inline vec2 operator+(const vec2& v) const {
		return vec2(x + v.x, y + v.y);
	}
	constexpr inline vec2 operator-(const vec2& v) const {
		return vec2(x - v.x, y - v.y);
	}
	constexpr inline vec2 operator*(const vec2& v) const {
		return vec2(x * v.x, y * v.y);
	}
	constexpr inline vec2 operator/(const vec2& v) const {
		return vec2(x / v.x, y / v.y);
	}

	// implicit conversion from float
	constexpr inline vec2 operator+(float f) const {
		return vec2(x + f, y + f);
	}
	constexpr inline vec2 operator-(float f) const {
		return vec2(x - f, y - f);
	}
	constexpr inline vec2 operator*(float f) const {
		return vec2(x * f, y * f);
	}
	constexpr inline vec2 operator/(float f) const {
		return vec2(x / f, y / f);
	}
};