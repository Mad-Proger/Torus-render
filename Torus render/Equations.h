#pragma once
#define _USE_MATH_DEFINES
#include "Complex.h"
#include <cmath>
#include <vector>

namespace Equations {
	const long double EPS = 1e-8;
	std::vector<Complex> solveQuadratic(const Complex &a, const Complex &b, const Complex &c);

	std::vector<Complex> solveCubic(const Complex &a, const Complex &b, const Complex &c, const Complex &d);
	std::vector<Complex> solveCubicDepressed(const Complex &p, const Complex &q);

	std::vector<Complex> solveQuartic(const Complex &a, const Complex &b, const Complex &c, const Complex &d, const Complex &e);
}