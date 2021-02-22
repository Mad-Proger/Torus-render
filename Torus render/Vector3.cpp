#include "Vector3.h"

Vector3::Vector3() {
	x = 0.L;
	y = 0.L;
	z = 0.L;
}

Vector3::Vector3(long double x, long double y, long double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::operator+(const Vector3 &a) const {
	return Vector3(x + a.x, y + a.y, z + a.z);
}

Vector3 Vector3::operator-(const Vector3 &a) const {
	return Vector3(x - a.x, y - a.y, z - a.z);
}

Vector3 Vector3::operator*(const long double &a) const {
	return Vector3(x * a, y * a, z * a);
}

long double Vector3::operator*(const Vector3 &a) const {
	return x * a.x + y * a.y + z * a.z;
}

Vector3 Vector3::operator%(const Vector3 &a) const {
	return Vector3(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
}

Vector3 Vector3::operator/(const long double &a) const {
	return Vector3(x / a, y / a, z / a);
}

Vector3 Vector3::operator+=(const Vector3 &a) {
	*this = this->operator+(a);
	return *this;
}

Vector3 Vector3::operator-=(const Vector3 &a) {
	*this = this->operator-(a);
	return *this;
}

Vector3 Vector3::operator*=(const long double &k) {
	*this = this->operator*(k);
	return *this;
}

Vector3 Vector3::operator/=(const long double &k) {
	*this = this->operator/(k);
	return *this;
}

Vector3 Vector3::operator+() const {
	return Vector3(x, y, z);
}

Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

long double Vector3::abs() const {
	return std::hypot(x, y, z);
}

long double Vector3::abs2() const {
	return x * x + y * y + z * z;
}

Vector3 Vector3::toNewBasis(const Vector3 &vx, const Vector3 &vy, const Vector3 &vz) const {
	return vx * x + vy * y + vz * z;
}

std::array<Vector3, 3> Vector3::rotationMatrixX(const long double &angle) {
	std::array<Vector3, 3> matrix;
	matrix[0] = Vector3(1.L, 0.L, 0.L);
	matrix[1] = Vector3(0.L, cosl(angle), -sinl(angle));
	matrix[2] = Vector3(0.L, sinl(angle), cosl(angle));
	return matrix;
}

std::array<Vector3, 3> Vector3::rotationMatrixY(const long double &angle) {
	std::array<Vector3, 3> matrix;
	matrix[0] = Vector3(cosl(angle), 0.L, sinl(angle));
	matrix[1] = Vector3(0.L, 1.L, 0.L);
	matrix[2] = Vector3(-sinl(angle), 0.L, cosl(angle));
	return matrix;
}

std::array<Vector3, 3> Vector3::rotationMatrixZ(const long double &angle) {
	std::array<Vector3, 3> matrix;
	matrix[0] = Vector3(cosl(angle), -sinl(angle), 0.L);
	matrix[1] = Vector3(sinl(angle), cosl(angle), 0.L);
	matrix[2] = Vector3(0.L, 0.L, 1.L);
	return matrix;
}

std::array<Vector3, 3> Vector3::rotationMatrix(const Vector3 &a, const Vector3 &b) {
	const long double angle = atan2l((a % b).abs(), a * b);
	Vector3 c = a % b;
	c.normalize();
	long double cosValue = cosl(angle);
	long double sinValue = sinl(angle);

	std::array<Vector3, 3> matrix;
	matrix[0] = Vector3(
		cosValue + (1.L - cosValue) * c.x * c.x,
		(1.L - cosValue) * c.x * c.y - sinValue * c.z,
		(1.L - cosValue) * c.x * c.z + sinValue * c.y);
	matrix[1] = Vector3(
		(1.L - cosValue) * c.y * c.x + sinValue * c.z,
		cosValue + (1.L - cosValue) * c.y * c.y,
		(1.L - cosValue) * c.y * c.z - sinValue * c.x);
	matrix[2] = Vector3(
		(1.L - cosValue) * c.z * c.x - sinValue * c.y,
		(1.L - cosValue) * c.z * c.y + sinValue * c.x,
		cosValue + (1.L - cosValue) * c.z * c.z);

	return matrix;
}

void Vector3::transform(const std::array<Vector3, 3> &matrix) {
	long double resultX = this->operator*(matrix[0]);
	long double resultY = this->operator*(matrix[1]);
	long double resultZ = this->operator*(matrix[2]);
	this->x = resultX;
	this->y = resultY;
	this->z = resultZ;
}

void Vector3::normalize() {
	operator/=(abs());
}

std::ostream &operator<<(std::ostream &out, const Vector3 &v) {
	return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

std::istream &operator>>(std::istream &in, Vector3 &v) {
	return in >> v.x >> v.y >> v.z;
}

Vector3 operator*(const long double &k, const Vector3 &v) {
	return Vector3(k * v.x, k * v.y, k * v.z);
}
