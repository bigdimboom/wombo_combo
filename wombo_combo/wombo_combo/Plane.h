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
	//Basic Methods
	inline float Distance(const point3& point) const;
	inline float PlaneDotCoord(const point3& point) const;
	inline point3 ProjPointFrom(const point3& point) const;
public:
	//Testing Methods
	bool IsPointInstersect(const point3& point) const;
	bool IsCubeInstersect(const point3& center,
		const float radius) const;
	static point3 GetPointFromCube(point3 center, float radius, int i);
private:
	point4 _plane;
};

