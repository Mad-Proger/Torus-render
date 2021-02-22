#include "Torus.h"
#include <iostream>

Torus::Torus() {
	this->center = Vector3();
	this->normal = Vector3(0, 1, 0);
	this->radius = 0.L;
	this->distance = 0.L;
}

Torus::Torus(Vector3 center, long double radius, long double distance) {
	this->center = center;
	this->normal = Vector3(0, 1, 0);
	this->radius = radius;
	this->distance = distance;
}

void Torus::render(const Vector3 &lightSrc, const Vector3 &viewPosition, long double fov, unsigned char *pixelArray, unsigned int width, unsigned int height) const {
	Vector3 vertical(0, -1, 0);
	Vector3 xDirection = center - viewPosition;
	xDirection.normalize();
	Vector3 zDirection = xDirection % vertical;
	zDirection.normalize();
	Vector3 yDirection = xDirection % zDirection;
	yDirection.normalize();

	long double hfov = fov * ((long double)height / width);
	Vector3 cornerDirection(1.L, 0.L, 0.L);
	cornerDirection.transform(Vector3::rotationMatrixY(-fov / 2.L));
	cornerDirection.transform(Vector3::rotationMatrixZ(hfov / 2.L));

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Vector3 pixelDirection = cornerDirection;
			pixelDirection.transform(Vector3::rotationMatrixY(fov * x / (long double)width));
			pixelDirection.transform(Vector3::rotationMatrixZ(-hfov * y / (long double)height));

			Vector3 pixelVector = pixelDirection.toNewBasis(xDirection, yDirection, zDirection);
			long double pixelBrightness = getBrightness(viewPosition, pixelVector, lightSrc);
			unsigned char pixelColorValue = unsigned char(std::min(255.L, std::max(0.L, pixelBrightness * 255.L)));

			unsigned int pixelIndex = y * width + x;
			pixelArray[4 * pixelIndex + 0] = pixelColorValue;
			pixelArray[4 * pixelIndex + 1] = pixelColorValue;
			pixelArray[4 * pixelIndex + 2] = pixelColorValue;
			pixelArray[4 * pixelIndex + 3] = 255;
		}
	}
}

void Torus::render1(const Vector3 &lightSrc, const Vector3 &viewPosition, long double fov, unsigned char *pixelArray, unsigned int width, unsigned int height) const {
	Vector3 vertical(0, -1, 0);
	Vector3 xDirection = center - viewPosition;
	xDirection.normalize();
	Vector3 zDirection = xDirection % vertical;
	zDirection.normalize();
	Vector3 yDirection = xDirection % zDirection;
	yDirection.normalize();

	long double hfov = fov * ((long double)height / width);
	Vector3 cornerDirection(1.L, 0.L, 0.L);
	cornerDirection.transform(Vector3::rotationMatrixY(-fov / 2.L));
	cornerDirection.transform(Vector3::rotationMatrixZ(hfov / 2.L));

	for (unsigned int x = 0; x < width; ++x) {
		for (unsigned int y = 0; y < height; ++y) {
			Vector3 pixelDirection = cornerDirection;
			pixelDirection.transform(Vector3::rotationMatrixY(fov * x / (long double)width));
			pixelDirection.transform(Vector3::rotationMatrixZ(-hfov * y / (long double)height));

			Vector3 pixelVector = pixelDirection.toNewBasis(xDirection, yDirection, zDirection);
			long double pixelBrightness = getBrightness1(viewPosition, pixelVector, lightSrc);
			unsigned char pixelColorValue = unsigned char(std::min(255.L, std::max(0.L, pixelBrightness * 255.L)));

			unsigned int pixelIndex = y * width + x;
			pixelArray[4 * pixelIndex + 0] = pixelColorValue;
			pixelArray[4 * pixelIndex + 1] = pixelColorValue;
			pixelArray[4 * pixelIndex + 2] = pixelColorValue;
			pixelArray[4 * pixelIndex + 3] = 255;
		}
	}
}

long double Torus::getBrightness(Vector3 pos, Vector3 direction, Vector3 lightPos) const {
	direction /= direction.abs();
	for (int i = 0; i < ITERATIONS; ++i) {
		long double currentDistance = getDistance(pos);
		if (currentDistance > INF)
			return 0.L;
		pos += direction * getDistance(pos);
	}
	if (abs(getDistance(pos)) > EPS)
		return 0.L;

	Vector3 rayDirection = pos - lightPos;
	rayDirection.normalize();
	for (int i = 0; i < ITERATIONS; ++i)
		lightPos += rayDirection * getDistance(lightPos);
	if ((lightPos - pos).abs() > 10.L * EPS)
		return 0.L;
	return -rayDirection * normalVector(pos);
}

Vector3 Torus::intersectCenteredTorus(const Vector3 &position, const Vector3 &direction) const {
	long double a = direction.abs2();
	long double b = 2.L * position * direction;
	long double c = position.abs2() + radius * radius - distance * distance;

	Vector3 centeredNormalVector(0.L, 0.L, 1.L);
	long double z0 = position * centeredNormalVector;
	long double z1 = direction * centeredNormalVector;
	long double d = a - z1 * z1;
	long double e = b - z0 * z1;
	long double f = c - z0 * z0;

	std::vector<Complex> equationRoots = Equations::solveQuartic(a * a, 2.L * a * b, 2.L * a * c + b * b - 4.L * radius * radius * d, 2.L * b * c - 4.L * radius * radius * e, c * c - 4.L * radius * radius * f);
	bool foundIntersection = false;
	long double intersectionParameter = 0.L;
	for (const Complex &root : equationRoots) {
		if (abs(root.imag()) > EPS)
			continue;
		if (root.real() < 0)
			continue;
		
		if (!foundIntersection) {
			foundIntersection = true;
			intersectionParameter = root.real();
		} else {
			intersectionParameter = std::min(intersectionParameter, root.real());
		}
	}

	if (foundIntersection)
		return position + intersectionParameter * direction;

	return Vector3();
}

Vector3 Torus::intersectTorus(const Vector3 &position, const Vector3 &direction) const {
	Vector3 targetNormalVector(0.L, 0.L, 1.L);
	std::array<Vector3, 3> transformationMatrix = Vector3::rotationMatrix(normal, targetNormalVector);
	std::array<Vector3, 3> inverseTransformationMatrix = Vector3::rotationMatrix(targetNormalVector, normal);

	Vector3 transformedPos = position - center;
	transformedPos.transform(transformationMatrix);
	Vector3 transformedDirection = direction;
	transformedDirection.transform(transformationMatrix);

	Vector3 intersectionPoint = intersectCenteredTorus(transformedPos, transformedDirection);
	intersectionPoint.transform(inverseTransformationMatrix);
	intersectionPoint += center;

	return intersectionPoint;
}

long double Torus::getBrightness1(const Vector3 &pos, const Vector3 &direction, const Vector3 &lightPos) const {
	Vector3 viewTorusIntersection = intersectTorus(pos, direction);
	if ((viewTorusIntersection - center).abs() < EPS)
		return 0.L;

	Vector3 lightTorusIntersection = intersectTorus(lightPos, viewTorusIntersection - lightPos);
	if ((viewTorusIntersection - lightTorusIntersection).abs() > EPS)
		return 0.L;

	Vector3 lightRayDirection = lightTorusIntersection - lightPos;
	lightRayDirection.normalize();
	return -lightRayDirection * normalVector(viewTorusIntersection);
}

void Torus::rotateX(long double angle) {
	normal.transform(Vector3::rotationMatrixX(angle));
}

void Torus::rotateY(long double angle) {
	normal.transform(Vector3::rotationMatrixY(angle));
}

void Torus::rotateZ(long double angle) {
	normal.transform(Vector3::rotationMatrixZ(angle));
}

long double Torus::getDistance(const Vector3 &p) const {
	Vector3 op = p - center;
	long double d = op * normal;
	Vector3 op1 = op - normal * d;
	Vector3 ox = op1 * (radius / op1.abs());
	Vector3 xp = p - (center + ox);
	return xp.abs() - distance;
}

Vector3 Torus::normalVector(const Vector3 &p) const {
	Vector3 op = p - center;
	long double d = op * normal;
	Vector3 op1 = op - normal * d;
	Vector3 ox = op1 * (radius / op1.abs());
	Vector3 xp = p - (center + ox);
	return xp / xp.abs();
}
