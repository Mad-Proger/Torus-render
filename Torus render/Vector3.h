#pragma once
#include <cmath>
#include <array>
#include <istream>
#include <ostream>

class Vector3 {
private:
	long double x, y, z;

public:
	Vector3();
	Vector3(long double x, long double y, long double z);

	Vector3 operator+(const Vector3 &a) const;
	Vector3 operator-(const Vector3 &a) const;
	Vector3 operator*(const long double &a) const;
	long double operator*(const Vector3 &a) const;
	Vector3 operator%(const Vector3 &a) const;
	Vector3 operator/(const long double &a) const;

	Vector3 operator+=(const Vector3 &a);
	Vector3 operator-=(const Vector3 &a);
	Vector3 operator*=(const long double &k);
	Vector3 operator/=(const long double &k);

	Vector3 operator+() const;
	Vector3 operator-() const;

	long double abs() const;
	long double abs2() const;

	Vector3 toNewBasis(const Vector3 &vx, const Vector3 &vy, const Vector3 &vz) const;
	static std::array<Vector3, 3> rotationMatrixX(const long double &angle);
	static std::array<Vector3, 3> rotationMatrixY(const long double &angle);
	static std::array<Vector3, 3> rotationMatrixZ(const long double &angle);
	static std::array<Vector3, 3> rotationMatrix(const Vector3 &a, const Vector3 &b);
	void transform(const std::array<Vector3, 3> &matrix);
	void normalize();

	friend std::ostream &operator<<(std::ostream &out, const Vector3 &v);
	friend std::istream &operator>>(std::istream &in, Vector3 &v);
	friend Vector3 operator*(const long double &k, const Vector3 &v);
};
