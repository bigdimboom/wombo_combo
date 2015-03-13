#include "Plane.h"

static float PlaneDotCoord(const point4 &plane, const point3 &vector)
{
	float a, b, c, d;
	a = plane.x;
	b = plane.y;
	c = plane.z;
	d = plane.w;

	return a*vector.x + b*vector.y + c*vector.z + d;
}


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
	_plane.x = (b.y - a.y) * (c.z - a.z) - (c.y - a.y)*(b.z - a.z);
	_plane.y = (b.z - a.z) * (c.x - a.x) - (c.z - a.z)*(b.x - a.x);
	_plane.z = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) *(b.y - a.y);
	_plane.w = -(_plane.x * a.x + _plane.y * a.y + _plane.z * a.z);

	Normalize();
}

bool Plane::Inside(const point3& point, const float radius) const
{
	float fDistance; 
	fDistance = PlaneDotCoord(_plane, point);
	return (fDistance >= -radius);
}

bool Plane::Inside(const point3& point) const
{
	float result = PlaneDotCoord(_plane, point);
	return (result >= 0.0f);
}