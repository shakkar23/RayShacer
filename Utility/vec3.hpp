#pragma once

#include "math.hpp"
#include <stdexcept>

namespace constants {

	constexpr double e =  2.71828182845904523536;
	constexpr double log2e =  1.44269504088896340736;
	constexpr double log10e =  0.434294481903251827651;
	constexpr double ln2 =  0.693147180559945309417;
	constexpr double ln10 =  2.30258509299404568402;
	constexpr double pi =  3.14159265358979323846;
	constexpr double pi_2 =  1.57079632679489661923;
	constexpr double pi_4 =  0.785398163397448309616;
	constexpr double sqrt2 =  1.41421356237309504880;
	constexpr double sqrt1_2 =  0.70710678118654752440;
};


namespace mth {
template <typename Real>
constexpr bool is_close(Real x, Real y) {
	return abs(x - y) <= 1e-7;
}

template <typename Real>
constexpr Real sqrt_newton(Real x, Real guess) {
	return is_close(guess * guess, x)
		? guess
		: sqrt_newton(x, (guess + x / guess) / static_cast<Real>(2));
}

template <typename Integer>
constexpr Integer range_product(Integer start, Integer end) {
	return (start > end) ? 1 : start * range_product(start + 1, end);
}


template <typename Integer>
constexpr Integer factorial(Integer n) {
	static_assert(std::is_integral<Integer>::value,
		"factorial is defined only for non-negative integers");
	// clang-format off
	return (n < 0) ? throw std::domain_error("n >= 0") :
		(n > 0) ? n * factorial(n - 1) : 1;
	// clang-format on
}

template <typename Real>
constexpr Real abs(Real num) {
	return (num < 0) ? -num : num;
}

template <typename Integer>
constexpr bool is_even(Integer num) {
	static_assert(std::is_integral<Integer>::value,
		"is_even is defined only for integer types");
	return num % 2 == 0;
}

template <typename Integer>
constexpr bool is_odd(Integer num) {
	static_assert(std::is_integral<Integer>::value,
		"is_odd is defined only for integer types");
	return num % 2 != 0;
}

template <typename Real>
constexpr bool is_negative(Real num) {
	return num < 0;
}

template <typename Real, typename Integer>
constexpr Real pow(Real a, Integer n) {
	static_assert(std::is_integral<Integer>::value,
		"pow supports only integral powers");
	// clang-format off
	return (n < 0) ? 1 / pow(a, -n) :
		(n == 0) ? 1 :
		(n == 1) ? a :
		(a == 2) ? 1LL << n :
		(is_even(n)) ? pow(a * a, n / 2) :
		(true) ? a * pow(a * a, (n - 1) / 2) : 0;
	// clang-format on
}

template <typename Real>
constexpr Real max(Real a, Real b) {
	return (a < b) ? b : a;
}

template <typename Real, class Compare>
constexpr Real max(Real a, Real b, Compare comp) {
	return comp(a, b) ? b : a;
}

template <typename Real>
constexpr Real min(Real a, Real b) {
	return (a < b) ? a : b;
}

template <typename Real, class Compare>
constexpr Real min(Real a, Real b, Compare comp) {
	return comp(a, b) ? a : b;
}

template <typename Real>
constexpr Real sqrt(Real x) {
	return (x < 0) ? throw std::domain_error("x >= 0")
		: mth::sqrt_newton(x, x);
}

template <typename Real>
constexpr Real hypot(Real a, Real b) {
	return mth::sqrt_newton(a * a + b * b, abs(a) + abs(b));
}

template <typename Real>
constexpr Real sqr(Real x) {
	return x * x;
}

template <typename Integer>
constexpr Integer choose(Integer n, Integer k) {
	static_assert(std::is_integral<Integer>::value,
		"choose works only for integral arguments");
	// clang-format off
	return (n < 0) ? throw std::domain_error("n >= 0") :
		(k < 0) ? throw std::domain_error("k >= 0") :
		(k > n) ? throw std::domain_error("k <= n") :
		mth::range_product(max(n - k, k) + 1, n) / factorial(min(n - k, k));
	// clang-format on
}

template <typename Integer>
constexpr Integer fibonacci(Integer iter, Integer a, Integer b) {
	static_assert(std::is_integral<Integer>::value,
		"fibonacci is defined only for integral inputs");
	return (iter <= 0) ? throw std::domain_error("n > 0")
		: (iter == 1) ? b : fibonacci(iter - 1, b, a + b);
}

template <typename Real>
constexpr Real deg2rad(Real deg) {
	return deg * (constants::pi / 180);
}

template <typename Real>
constexpr Real rad2deg(Real rad) {
	return rad * (180 / constants::pi);
}

template <typename Floating, typename Integer>
constexpr Integer floor(Floating x) {
	static_assert(std::is_floating_point<Floating>::value,
		"floor accepts only floating point inputs");
	return static_cast<Integer>(x) - (static_cast<Integer>(x) > x);
}

template <typename Floating, typename Integer>
constexpr Integer ceil(Floating x) {
	static_assert(std::is_floating_point<Floating>::value,
		"floor accepts only floating point inputs");
	return static_cast<Integer>(x) + (static_cast<Integer>(x) < x);
}



template <typename Real>
constexpr Real wrap(Real x) {
	// standardize the angle so that -pi <= x < pi
	// clang-format off
	return (x <= -constants::pi) ? wrap(x + 2 * constants::pi) :
		(x > constants::pi) ? wrap(x - 2 * constants::pi) :
		(true) ? x : 0;
	// clang-format on
}

template <typename Real>
constexpr Real sin_cfrac(Real x2, int k = 2, int n = 40) {
	return (n == 0) ? k * (k + 1) - x2
		: k * (k + 1) - x2 +
		(k * (k + 1) * x2) / sin_cfrac(x2, k + 2, n - 1);
}

template <typename Real>
constexpr Real tan_cfrac(Real x2, int k = 1, int n = 40) {
	return (n == 0) ? k : k - x2 / tan_cfrac(x2, k + 2, n - 1);
}

template <typename Real>
constexpr Real fast_atan_unit(Real x) {  // -1 <= x <= 1
	return constants::pi_4 * x - x * (abs(x) - 1) * (0.2447 + 0.0663 * abs(x));
}

template <typename Real>
constexpr Real sin(Real x) {
	return mth::wrap(x) /
		(1 + sqr(mth::wrap(x)) / mth::sin_cfrac(sqr(mth::wrap(x))));
}

template <typename Real>
constexpr Real cos(Real x) {
	return sin(constants::pi_2 - x);
}

template <typename Real>
constexpr Real tan(Real x) {
	return mth::wrap(x) / mth::tan_cfrac(sqr(mth::wrap(x)));
}

template <typename Real>
constexpr Real csc(Real x) {
	return 1.0 / sin(x);
}

template <typename Real>
constexpr Real sec(Real x) {
	return 1.0 / cos(x);
}

template <typename Real>
constexpr Real cot(Real x) {
	return 1.0 / tan(x);
}

template <typename Real>
constexpr Real fast_atan(Real x) {
	return (abs(x) <= 1) ? mth::fast_atan_unit(x)
		: constants::pi_2 - mth::fast_atan_unit(1.0 / x);
}

};


namespace shk {
	constexpr auto sqrt(auto x) {
		if (std::is_constant_evaluated())
			return mth::sqrt(x);
		else 
			return std::sqrt(x);
	}

	constexpr auto cos(auto x) {
		if (std::is_constant_evaluated())
			return mth::cos(x);
		else
			return std::cos(x);
	}

	constexpr auto sin(auto x) {
		if (std::is_constant_evaluated())
			return mth::sin(x);
		else
			return std::sin(x);
	}
};


#include <cmath>
#include <type_traits>

class vec3 {
public:
	float x, y, z;
	constexpr vec3() : x(0), y(0), z(0) {}
	constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	constexpr vec3(float v) : x(v), y(v), z(v) {}
	constexpr ~vec3() = default;

	constexpr vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}
	constexpr vec3(vec3&& v) noexcept : x(v.x), y(v.y), z(v.z) {}
	// copy assignment operator
	constexpr vec3& operator=(const vec3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}


	constexpr inline void normalizeThis() {
		float l = length();
		if (l > 0) {
			x /= l;
			y /= l;
			z /= l;
		}
	}
	constexpr inline vec3 normalize() const {
		float l = length();
		if (l > 0) {
			return vec3(x / l, y / l, z / l);
		}
		return vec3(0, 0, 0);
	}
	constexpr inline vec3 rotateYP(float yaw, float pitch) const {
		// Convert to radians
		double yawRads = toRadians(yaw);
		double pitchRads = toRadians(pitch);
		
		// Step one: Rotate around X axis (pitch)
		float _y = (float)(y * shk::cos(pitchRads) - z * shk::sin(pitchRads));
		float _z = (float)(y * shk::sin(pitchRads) + z * shk::cos(pitchRads));

		// Step two: Rotate around the Y axis (yaw)
		float _x = (float)(x * shk::cos(yawRads) + _z * shk::sin(yawRads));
			  _z = (float)(-x * shk::sin(yawRads) + _z * shk::cos(yawRads));

		return vec3(_x, _y, _z);
	}
	constexpr inline float length() const {
		return shk::sqrt(x * x + y * y + z * z);
	}
	constexpr inline float cross(const vec3& v) const {
		return x * v.y - y * v.x;
	}
	constexpr inline vec3 abs() {
		return vec3(x < 0 ? -x : x, y < 0 ? -y : y, z < 0 ? -z : z);
	}
	constexpr inline float min_element() const {
		return mth::min(mth::min(x, y), z);
	}
	constexpr inline vec3 floor() const {
		return vec3(float(int(x)), float(int(y)), float(int(z)));
	}
	constexpr inline vec3 fract() const {
		vec3 absed = vec3(x, y, z) * vec3(x, y, z).signum();
		return vec3(absed.x - float(int(absed.x)), absed.y - float(int(absed.y)), absed.z - float(int(absed.z)));
	}
	constexpr inline vec3 signum() const {
		return vec3((x >= 0.0) ? 1.0 : -1.0, (y >= 0.0) ? 1.0 : -1.0, (z >= 0.0) ? 1.0 : -1.0);
	}
	constexpr inline vec3 sqrt() const {
		return vec3(shk::sqrt(x), shk::sqrt(y), shk::sqrt(z));
	}

	//operators
	constexpr inline vec3 operator+(const vec3& v) const {
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	constexpr inline vec3 operator-(const vec3& v) const {
		return vec3(x - v.x, y - v.y, z - v.z);
	}
	constexpr inline vec3 operator*(const vec3& v) const {
		return vec3(x * v.x, y * v.y, z * v.z);
	}
	constexpr inline vec3 operator/(const vec3& v) const {
		return vec3(x / v.x, y / v.y, z / v.z);
	}

	// implicit conversion from float to vec3
	constexpr inline vec3 operator*(float f) const {
		return vec3(x * f, y * f, z * f);
	}
	constexpr inline vec3 operator/(float f) const {
		return vec3(x / f, y / f, z / f);
	}
	constexpr inline vec3 operator+(float f) const {
		return vec3(x + f, y + f, z + f);
	}
	constexpr inline vec3 operator-(float f) const {
		return vec3(x - f, y - f, z - f);
	}
	constexpr inline vec3 operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	constexpr inline vec3 operator-=(const vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	constexpr inline vec3 operator*=(const vec3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	constexpr inline vec3 operator/=(const vec3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	constexpr inline vec3 operator+=(float f) {
		x += f;
		y += f;
		z += f;
		return *this;
	}
	constexpr inline vec3 operator-=(float f) {
		x -= f;
		y -= f;
		z -= f;
		return *this;
	}
	constexpr inline vec3 operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	constexpr inline vec3 operator/=(float f) {
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
	//negation
	constexpr inline vec3 operator-() const {
		return vec3(-x, -y, -z);
	}
	constexpr inline bool operator>(vec3 in) {
		return this->length() > in.length();
	}
	constexpr inline bool operator<(vec3 in) {
		return this->length() < in.length();
	}
	constexpr inline bool operator>=(vec3 in) {
		return this->length() >= in.length();
	}
	constexpr inline bool operator<=(vec3 in) {
		return this->length() <= in.length();
	}
	constexpr inline bool operator==(vec3 in) {
		return x == in.x && y == in.y && z == in.z;
	}
	constexpr inline bool operator!=(vec3 in) {
		return x != in.x || y != in.y || z != in.z;
	}
	constexpr inline bool operator[](int in) {
		return in == 0 ? x : in == 1 ? y : z;
	}

	// dotThis product between two vectors
	constexpr inline float dotThis(const vec3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

private:


};

// float divided by vec3
constexpr inline vec3 operator/(float scalar, const vec3& vec) {
	return vec3(scalar / vec.x, scalar / vec.y, scalar / vec.z);
}
