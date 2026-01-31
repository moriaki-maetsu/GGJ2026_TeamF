#include "Geometry.h"
#include <cmath>

const float Vector2D::Magnitude() const
{
	return std::hypot(x, y);
}

void Vector2D::Normalize()
{
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2D Vector2D::Normalized() const
{
	float mag = Magnitude();
	return Vector2D(x / mag, y / mag);
}

Vector2D Vector2D::Lerp(const Vector2D& va, const Vector2D& vb, float t)
{
	return (va + ((vb - va) * t));
}

Vector2D Vector2D::Reflect(const Vector2D& v, const Vector2D& n)
{
	return (v - 2.0f * Vector2D::Dot(v, n) * n);
}
