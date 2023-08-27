#pragma once

#include "math.hpp"
#include <cmath>


class vec3 {
public:
	float x, y, z;
	vec3() : x(0), y(0), z(0) {}
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3(float v) : x(v), y(v), z(v) {}

	void normalizeThis();
	
	vec3 normalize() const;

	
	vec3 rotateYP(float yaw, float pitch);

	
	float length() const;

	float cross(const vec3& v);

	vec3 abs();

	float min_element() const;

	vec3 floor() const;

	vec3 fract() const;

	vec3 signum() const;
	vec3 sqrt() const;

	//operators
	vec3 operator+(const vec3& v) const;
	vec3 operator-(const vec3& v) const;
	vec3 operator*(const vec3& v) const;
	vec3 operator/(const vec3& v) const;

	// implicit conversion from float to vec3
	vec3 operator*(float f) const;
	vec3 operator/(float f) const;
	vec3 operator+(float f) const;
	vec3 operator-(float f) const;
	

	vec3 operator+=(const vec3& v);
	vec3 operator-=(const vec3& v);
	vec3 operator*=(const vec3& v);
	vec3 operator/=(const vec3& v);

	vec3 operator+=(float f);
	vec3 operator-=(float f);
	vec3 operator*=(float f);
	vec3 operator/=(float f);
	

	//negation
	vec3 operator-() const	;
	bool operator>(vec3 in);
	bool operator<(vec3 in);
	bool operator>=(vec3 in);
	bool operator<=(vec3 in);
	bool operator==(vec3 in);
	bool operator!=(vec3 in);

	bool operator[](int in);
	
	// dotThis product between two vectors
	float dotThis(const vec3& v) const;
	private :
		

};
	// float divided by vec3
	 vec3 operator/(float scalar, const vec3& vec);