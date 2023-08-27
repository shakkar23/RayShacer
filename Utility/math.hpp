#pragma once




constexpr inline float toRadians(float degrees) {
	constexpr auto pi = 3.14159265358979323846;
	return degrees * (pi / 180);
}