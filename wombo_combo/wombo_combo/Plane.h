#pragma once
#include "Global.h"

class Plane
{
public:
	Plane();
	Plane(const point3& p0, const point3& p1, const point3& p2);
	~Plane();
	void Set(const point3& p0, const point3& p1, const point3& p2);
	inline point4 Get() const { return _plane; }
	inline point3 GetNormal() const { return point3(_plane); }
public:
	float Distance(point3& point);
	float PlaneDotCoord(point3& point);
	point3 ProjPointFrom(point3& point);
private:
	point4 _plane;
};

