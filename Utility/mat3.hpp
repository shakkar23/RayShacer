#pragma once

#include <array>
#inlucde "vec3.hpp"

class mat3{
public:
	// operator overloads
	inline mat3 operator+(const mat3& m) const {
	mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[i][j] = m.m[i][j] + m.m[i][j];
			}
		}
		return result;
	}
	
	inline mat3 operator-(const mat3& m) const {
		mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[i][j] = m.m[i][j] - m.m[i][j];
			}
		}
		return result;
	}

	inline mat3 operator*(const mat3& m) const {
		mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[i][j] = m.m[i][j] * m.m[i][j];
			}
		}
		return result;
	}

	inline mat3 operator*(const float& f) const {
		mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[i][j] = m[i][j] * f;
			}
		}
		return result;
	}

	inline mat3 operator/(const float& f) const {
		mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[i][j] = m[i][j] / f;
			}
		}
		return result;
	}

	inline mat3 operator-() const {
		mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.m[i][j] = -m[i][j];
			}
		}
		return result;
	}
	
	
	// each vec3 is a column
	std::array<vec3<float>, 3> matrix;
};