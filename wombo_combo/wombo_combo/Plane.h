#pragma once

#include "Global.h"

class Plane
{
public:
	Plane();
	Plane(const point3 &p0, const point3 &p1, const point3 &p2);
	~Plane();
	inline void Normalize();
	inline void Set(const point3 &p0, const point3 &p1, const point3 &p2);

	bool Inside(const point3& point, const float radius);
	bool Inside(const point3& point);

	inline point4* Get() { return &_plane; }
	inline point3 GetNormal() { return point3(_plane); }

	float PlaneDotCoord(const point3& vector);

private:
	point4 _plane;
};

