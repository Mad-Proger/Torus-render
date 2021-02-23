#pragma once
#include <cmath>
#include "Vector3.h"
#include "Equations.h"

class Torus {
private:
	const long double EPS = 1e-2;
	Vector3 center;
	Vector3 normal;
	long double radius;
	long double distance;

	Vector3 normalVector(const Vector3 &p) const;
	Vector3 intersectCenteredTorus(const Vector3 &position, const Vector3 &direction) const;
	Vector3 intersectTorus(const Vector3 &position, const Vector3 &direction) const;
	long double getBrightness(const Vector3 &pos, const Vector3 &direction, const Vector3 &lightPos) const;

public:
	Torus();
	Torus(Vector3 center, long double radius, long double distance);

	void render(const Vector3 &lightSrc, const Vector3 &viewPosition, long double fov, unsigned char *pixelArray, unsigned int width, unsigned int height) const;

	void rotateX(long double angle);
	void rotateY(long double angle);
	void rotateZ(long double angle);
};
