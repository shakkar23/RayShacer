#pragma once 
#include <SDl.h>



struct ShakColor {
	ShakColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) : b(b), g(g), r(r), a(a) {}
	ShakColor() {}
	Uint8 b, g, r, a;

};