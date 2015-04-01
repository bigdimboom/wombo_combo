#include "Circle.h"


Circle::Circle(const point4& center,
	const point3& planeNormal,
	float radius)
	:_isBuilt(false)
{
}


Circle::~Circle()
{
}

void Circle::Init(const point4& center,
	const point3& planeNormal,
	float radius)
{
	assert(_isBuilt != true);
	point3 normal = glm::normalize(planeNormal);
}