#pragma once


class vec2 {
public:
	float x, y;
	inline vec2() {}
	inline vec2(float x, float y) : x(x), y(y) {}
	
	// operators
	inline vec2 operator+(const vec2& v) const {
		return vec2(x + v.x, y + v.y);
	}
	inline vec2 operator-(const vec2& v) const {
		return vec2(x - v.x, y - v.y);
	}
	inline vec2 operator*(const vec2& v) const {
		return vec2(x * v.x, y * v.y);
	}
	inline vec2 operator/(const vec2& v) const {
		return vec2(x / v.x, y / v.y);
	}

	// implicit conversion from float
	inline vec2 operator+(float f) const {
		return vec2(x + f, y + f);
	}
	inline vec2 operator-(float f) const {
		return vec2(x - f, y - f);
	}
	inline vec2 operator*(float f) const {
		return vec2(x * f, y * f);
	}
	inline vec2 operator/(float f) const {
		return vec2(x / f, y / f);
	}
};