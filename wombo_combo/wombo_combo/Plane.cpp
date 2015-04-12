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
	float l = glm::length(n);
	float D = -(n.x * p0.x + n.y * p0.y + n.z * p0.z);
	_plane.x = n.x / l;
	_plane.y = n.y / l;
	_plane.z = n.z / l;
	_plane.w = D / l;
	//_plane = glm::normalize(_plane);
}

float Plane::Distance(const point3& point) const
{
	//determining the plane's relationship 
	//with a coordinate in 3D space.
	return _plane.x * point.x +
		_plane.y * point.y +
		_plane.z * point.z +
		_plane.w;
}

point3 Plane::ProjPointFrom(const point3& point) const 
{
	return (point - Distance(point) * point3(_plane.x, _plane.y, _plane.z));
}

// 'distance: 0 -> on the plane'
// 'distace: > 0, in the plane(normal direction)'
// '<0, outside the plane'
bool Plane::IsPointInstersect(const point3& point) const
{
	if (Distance(point) >= 0)
	{
		return true;
	}
	return false;
}

bool Plane::IsCubeInstersect(const point3& center,
	const float radius) const
{
	
	for (int i = 0; i < 8; ++i)
	{
		if (IsPointInstersect(GetPointFromCube(center, radius, i)))
		{
			return true;
		}
	}
	return false;
}

bool Plane::IsSphereInstersect(const point3& center,
	const float radius) const
{
	if (Distance(center) >= -radius)
	{
		return true;
	}

	return false;
}


point3 Plane::GetPointFromCube(const point3& center, float radius, int i)
{
	switch (i)
	{
	case 0:
		return point3(center.x - radius, center.y + radius, center.z - radius);
	case 1:
		return point3(center.x + radius, center.y + radius, center.z - radius);
	case 2:
		return point3(center.x - radius, center.y - radius, center.z - radius);
	case 3:
		return point3(center.x + radius, center.y - radius, center.z - radius);
	case 4:
		return point3(center.x - radius, center.y + radius, center.z + radius);
	case 5:
		return point3(center.x + radius, center.y + radius, center.z + radius);
	case 6:
		return point3(center.x - radius, center.y - radius, center.z + radius);
	case 7:
		return point3(center.x + radius, center.y - radius, center.z + radius);
	default:
		std::cout << "Error passing in \n";
	}
	return point3(0.0f,0.0f,0.0f);
}