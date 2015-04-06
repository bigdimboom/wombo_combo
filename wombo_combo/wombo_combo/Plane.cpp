#include "Plane.h"


Plane::Plane()
	:_plane(0)
{
}

Plane::Plane(const point3 &p0, const point3 &p1, const point3 &p2)
{
	Set(p0, p1, p2);
}


Plane::~Plane()
{
}


inline void Plane::Normalize()
{
	_plane = glm::normalize(_plane);
}

inline void Plane::Set(const point3 &a, const point3 &b, const point3 &c)
{
	point3 u = b - a;
	point3 v = c - a;
	point3 n = glm::normalize(glm::cross(u, v));
	float D = -(n.x * a.x + n.y * a.y + n.z * a.z);

	_plane.x = n.x;
	_plane.y = n.y;
	_plane.z = n.z;
	_plane.w = D;

	Normalize();
}

bool Plane::IsInside(const point3& point, const float radius)
{
	float fDistance;
	fDistance = PlaneDotCoord(point);
	return (fDistance >= -radius);
}

bool Plane::IsInside(const point3& point)
{
	float result = PlaneDotCoord(point);
	return (result >= 0.0f);
}

float Plane::PlaneDotCoord(const point3& vector)
{
	return (glm::dot(point3(_plane), vector) + _plane.w);
}

point3 Plane::ProjectAPointFrom(const point3& point)
{
	return (point - PlaneDotCoord(point) * point3(_plane));
}