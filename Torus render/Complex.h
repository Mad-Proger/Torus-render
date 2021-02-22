#pragma once
#include <cmath>
#include <istream>
#include <ostream>

class Complex {
private:
	long double re;
	long double im;

public:
	Complex();
	Complex(const long double &real, const long double &imaginary);
	Complex(const long double &val);

	Complex operator+(const long double &a) const;
	Complex operator+(const Complex &a) const;
	Complex operator-(const long double &a) const;
	Complex operator-(const Complex &a) const;
	Complex operator*(const long double &a) const;
	Complex operator*(const Complex &a) const;
	Complex operator/(const long double &a) const;
	Complex operator/(const Complex &a) const;

	Complex operator+=(const long double &a);
	Complex operator+=(const Complex &a);
	Complex operator-=(const long double &a);
	Complex operator-=(const Complex &a);
	Complex operator*=(const long double &a);
	Complex operator*=(const Complex &a);
	Complex operator/=(const long double &a);
	Complex operator/=(const Complex &a);

	Complex operator+() const;
	Complex operator-() const;

	friend Complex operator+(const long double &a, const Complex &b);
	friend Complex operator-(const long double &a, const Complex &b);
	friend Complex operator*(const long double &a, const Complex &b);
	friend Complex operator/(const long double &a, const Complex &b);

	long double &real();
	const long double &real() const;
	long double &imag();
	const long double &imag() const;

	long double arg() const;
	long double abs() const;
	long double abs2() const;
	Complex conj() const;
	static Complex sqrt(const Complex &x);
	static Complex cbrt(const Complex &x);
	static Complex unit();

	friend std::ostream &operator<<(std::ostream &out, const Complex &z);
	friend std::istream &operator>>(std::istream &in, Complex &z);
};

