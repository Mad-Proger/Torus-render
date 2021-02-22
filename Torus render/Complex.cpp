#include "Complex.h"

Complex::Complex() {
	this->re = 0.L;
	this->im = 0.L;
}

Complex::Complex(const long double &real, const long double &imaginary) {
	this->re = real;
	this->im = imaginary;
}

Complex::Complex(const long double &val) {
	this->re = val;
	this->im = 0.L;
}

Complex Complex::operator+(const long double &a) const {
	return Complex(re + a, im);
}

Complex Complex::operator+(const Complex &a) const {
	return Complex(re + a.re, im + a.im);
}

Complex Complex::operator-(const long double &a) const {
	return Complex(re - a, im);
}

Complex Complex::operator-(const Complex &a) const {
	return Complex(re - a.re, im - a.im);
}

Complex Complex::operator*(const long double &a) const {
	return Complex(re * a, im * a);
}

Complex Complex::operator*(const Complex &a) const {
	return Complex(re * a.re - im * a.im, re * a.im + im * a.re);
}

Complex Complex::operator/(const long double &a) const {
	return Complex(re / a, im / a);
}

Complex Complex::operator/(const Complex &a) const {
	return this->operator*(a.conj()) / a.abs2();
}

Complex Complex::operator+=(const long double &a) {
	*this = this->operator+(a);
	return *this;
}

Complex Complex::operator+=(const Complex &a) {
	*this = this->operator+(a);
	return *this;
}

Complex Complex::operator-=(const long double &a) {
	*this = this->operator-(a);
	return *this;
}

Complex Complex::operator-=(const Complex &a) {
	*this = this->operator-(a);
	return *this;
}

Complex Complex::operator*=(const long double &a) {
	*this = this->operator*(a);
	return *this;
}

Complex Complex::operator*=(const Complex &a) {
	*this = this->operator*(a);
	return *this;
}

Complex Complex::operator/=(const long double &a) {
	*this = this->operator/(a);
	return *this;
}

Complex Complex::operator/=(const Complex &a) {
	*this = this->operator/(a);
	return *this;
}

Complex Complex::operator+() const {
	return Complex(re, im);
}

Complex Complex::operator-() const {
	return Complex(-re, -im);
}

long double &Complex::real() {
	return this->re;
}

const long double &Complex::real() const {
	return this->re;
}

long double &Complex::imag() {
	return this->im;
}

const long double &Complex::imag() const {
	return this->im;
}

long double Complex::arg() const {
	return atan2l(im, re);
}

long double Complex::abs() const {
	return hypotl(re, im);
}

long double Complex::abs2() const {
	return re * re + im * im;
}

Complex Complex::conj() const {
	return Complex(re, -im);
}

Complex Complex::sqrt(const Complex &x) {
	long double argNew = x.arg() / 2.L;
	return sqrtl(x.abs()) * Complex(cosl(argNew), sinl(argNew));
}

Complex Complex::cbrt(const Complex &x) {
	long double argNew = x.arg() / 3.L;
	return cbrtl(x.abs()) * Complex(cosl(argNew), sinl(argNew));
}

Complex Complex::unit() {
	return Complex(0.L, 1.L);
}

Complex operator+(const long double &a, const Complex &b) {
	return b.operator+(a);
}

Complex operator-(const long double &a, const Complex &b) {
	return -b.operator-(a);
}

Complex operator*(const long double &a, const Complex &b) {
	return b.operator*(a);
}

Complex operator/(const long double &a, const Complex &b) {
	return (a * b.conj()) / b.abs2();
}

std::ostream &operator<<(std::ostream &out, const Complex &z) {
	return out << z.re << " + " << z.im << " * i";
}

std::istream &operator>>(std::istream &in, Complex &z) {
	return in >> z.re >> z.im;
}
