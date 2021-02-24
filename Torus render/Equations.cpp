#include "Equations.h"

std::vector<Complex> Equations::solveQuadratic(const Complex &a, const Complex &b, const Complex &c) {
	Complex discriminantRoot = Complex::sqrt(b * b - 4 * a * c);
	return std::vector<Complex>{ (-b + discriminantRoot) / (2 * a), (-b - discriminantRoot) / (2 * a) };
}

std::vector<Complex> Equations::solveCubic(const Complex &a, const Complex &b, const Complex &c, const Complex &d) {
	Complex p = (3.L * a * c - b * b) / (3.L * a * a);
	Complex q = (2.L * b * b * b - 9.L * a * b * c + 27.L * a * a * d) / (27.L * a * a * a);
	std::vector<Complex> substitutionRoots = solveCubicDepressed(p, q);
	std::vector<Complex> variableRoots;
	for (const Complex &substitutionRoot : substitutionRoots)
		variableRoots.push_back(substitutionRoot - b / (3.L * a));
	return variableRoots;
}

std::vector<Complex> Equations::solveCubicDepressed(const Complex &p, const Complex &q) {
	Complex delta = (p * p * p) / 27.L + (q * q) / 4.L;
	Complex alpha = Complex::cbrt(-q / 2.L + Complex::sqrt(delta));
	Complex beta = (-p / 3.L) / alpha;
	
	std::vector<Complex> roots;
	roots.push_back(alpha + beta);
	roots.push_back(-(alpha + beta) / 2.L + Complex::unit() * (alpha - beta) * sqrtl(0.75L));
	roots.push_back(-(alpha + beta) / 2.L - Complex::unit() * (alpha - beta) * sqrtl(0.75L));
	return roots;
}

std::vector<Complex> Equations::solveQuartic(const Complex &a, const Complex &b, const Complex &c, const Complex &d, const Complex &e) {
	Complex p = (8.L * a * c - 3.L * b * b) / (8.L * a * a);
	Complex q = (b * b * b - 4.L * a * b * c + 8.L * a * a * d) / (8.L * a * a * a);
	
	Complex delta0 = c * c - 3.L * b * d + 12.L * a * e;
	Complex delta1 = 2.L * c * c * c - 9.L * b * c * d + 27.L * b * b * e + 27.L * a * d * d - 72.L * a * c * e;

	Complex t = Complex::cbrt((delta1 + Complex::sqrt(delta1 * delta1 - 4.L * delta0 * delta0 * delta0)) / 2.L);
	if (t.abs() < EPS)
		t = Complex::cbrt(Complex::sqrt(delta1 * delta1));
	Complex s = Complex::sqrt(-(2.L * p) / 3.L + (t + delta0 / t) / (3.L * a)) / 2.L;
	while (s.abs() < EPS) {
		t *= Complex(std::cos(2.L * M_PI / 3.L), std::sin(2.L * M_PI / 3.L));
		s = Complex::sqrt(-(2.L * p) / 3.L + (t + delta0 / t) / (3.L * a)) / 2.L;
	}

	std::vector<Complex> roots;
	Complex gamma0 = -b / (4.L * a);

	Complex rootGamma1 = Complex::sqrt(-4.L * s * s - 2.L * p + q / s) / 2.L;
	roots.push_back(gamma0 - s + rootGamma1);
	roots.push_back(gamma0 - s - rootGamma1);

	Complex rootGamma2 = Complex::sqrt(-4.L * s * s - 2.L * p - q / s) / 2.L;
	roots.push_back(gamma0 + s + rootGamma2);
	roots.push_back(gamma0 + s - rootGamma2);

	return roots;
}
