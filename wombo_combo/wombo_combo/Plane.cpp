#include "Plane.h"


Plane::Plane()
{
}

Plane::Plane(const point3& p0, const point3& p1, const point3& p2)
{
	Set(p0, p1, p2);
}


Plane::~Plane()
{
}

void Plane::Set(const point3& p0, const point3& p1, const point3& p2)
{
	point3 u = p1 - p0;
	point3 v = p2 - p0;
	point3 n = glm::normalize(glm::cross(u, v));
	float D = -(n.x * p0.x + n.y * p0.y + n.z * p0.z);
	_plane.x = n.x;
	_plane.y = n.y;
	_plane.z = n.z;
	_plane.w = D;
	_plane = glm::normalize(_plane);
}

/*
if it is zero is on the plane) also gives information 
as to which side of the plane is point r. 
If the sign is positive then the point is on the side that agrees with the normal n, otherwise it is on the other side.
*/

float Plane::Distance(point3& point)
{
	return PlaneDotCoord(point);
}

float Plane::PlaneDotCoord(point3& point)
{
	//determining the plane's relationship 
	//with a coordinate in 3D space.
	return _plane.x * point.x + 
		_plane.y * point.y + 
		_plane.z * point.z + 
		_plane.w;
}

point3 Plane::ProjPointFrom(point3& point)
{
	return (point - Distance(point) * point3(_plane.x, _plane.y, _plane.z));
}