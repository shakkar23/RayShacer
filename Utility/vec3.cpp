#include "vec3.hpp"

#include <cmath>


void vec3::normalizeThis() {
	float l = length();
	if (l > 0) {
		x /= l;
		y /= l;
		z /= l;
	}
}

vec3 vec3::normalize() const {
	float l = length();
	if (l > 0) {
		return vec3(x / l, y / l, z / l);
	}
	return vec3(0, 0, 0);
}


vec3 vec3::rotateYP(float yaw, float pitch) {
	// Convert to radians
	double yawRads = toRadians(yaw);
	double pitchRads = toRadians(pitch);

	// Step one: Rotate around X axis (pitch)
	float _y = (float)(y * cos(pitchRads) - z * sin(pitchRads));
	float _z = (float)(y * sin(pitchRads) + z * cos(pitchRads));

	// Step two: Rotate around the Y axis (yaw)
	float _x = (float)(x * cos(yawRads) + _z * sin(yawRads));
	_z = (float)(-x * sin(yawRads) + _z * cos(yawRads));

	return vec3(_x, _y, _z);
}


float vec3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}

float vec3::cross(const vec3& v) {
	return x * v.y - y * v.x;
}

vec3 vec3::abs() {
	return vec3(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z);
}

float vec3::min_element() const {
	return std::min(std::min(x, y), z);
}

vec3 vec3::floor() const {
	return vec3(std::floor(x), std::floor(y), std::floor(z));
}

vec3 vec3::fract() const {
	vec3 absed = vec3(x, y, z) * vec3(x, y, z).signum();
	return vec3(absed.x - std::floor(absed.x), absed.y - std::floor(absed.y), absed.z - std::floor(absed.z));
}

vec3 vec3::signum() const {
	return vec3((x >= 0.0) ? 1.0 : -1.0, (y >= 0.0) ? 1.0 : -1.0, (z >= 0.0) ? 1.0 : -1.0);
}
vec3 vec3::sqrt() const {
	return vec3(std::sqrt(x), std::sqrt(y), std::sqrt(z));
}

//operators
vec3 vec3::operator+(const vec3& v) const {
	return vec3(x + v.x, y + v.y, z + v.z);
}
vec3 vec3::operator-(const vec3& v) const {
	return vec3(x - v.x, y - v.y, z - v.z);
}
vec3 vec3::operator*(const vec3& v) const {
	return vec3(x * v.x, y * v.y, z * v.z);
}
vec3 vec3::operator/(const vec3& v) const {
	return vec3(x / v.x, y / v.y, z / v.z);
}

// implicit conversion from float to vec3
vec3 vec3::operator*(float f) const {
	return vec3(x * f, y * f, z * f);
}
vec3 vec3::operator/(float f) const {
	return vec3(x / f, y / f, z / f);
}
vec3 vec3::operator+(float f) const {
	return vec3(x + f, y + f, z + f);
}
vec3 vec3::operator-(float f) const {
	return vec3(x - f, y - f, z - f);
}


vec3 vec3::operator+=(const vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
vec3 vec3::operator-=(const vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
vec3 vec3::operator*=(const vec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}
vec3 vec3::operator/=(const vec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

vec3 vec3::operator+=(float f) {
	x += f;
	y += f;
	z += f;
	return *this;
}
vec3 vec3::operator-=(float f) {
	x -= f;
	y -= f;
	z -= f;
	return *this;
}
vec3 vec3::operator*=(float f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}
vec3 vec3::operator/=(float f) {
	x /= f;
	y /= f;
	z /= f;
	return *this;
}


//negation
vec3 vec3::operator-() const {
	return vec3(-x, -y, -z);
}
bool vec3::operator>(vec3 in) {
	return this->length() > in.length();
}
bool vec3::operator<(vec3 in) {
	return this->length() < in.length();
}
bool vec3::operator>=(vec3 in) {
	return this->length() >= in.length();
}
bool vec3::operator<=(vec3 in) {
	return this->length() <= in.length();
}
bool vec3::operator==(vec3 in) {
	return x == in.x && y == in.y && z == in.z;
}
bool vec3::operator!=(vec3 in) {
	return x != in.x || y != in.y || z != in.z;
}

bool vec3::operator[](int in) {
	return in == 0 ? x : in == 1 ? y : z;
}

// dotThis product between two vectors
float vec3::dotThis(const vec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

// float divided by vec3
vec3 operator/(float scalar, const vec3& vec) {
	return vec3(scalar / vec.x, scalar / vec.y, scalar / vec.z);
}