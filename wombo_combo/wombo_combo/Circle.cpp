#include "Circle.h"


Circle::Circle()
	:_isBuilt(false)
{
	Init();

}


Circle::~Circle()
{
}

void Circle::Init()
{
	assert(_isBuilt != true);
	point3  normal(0.0f, 1.0f, 0.0f);

	SetVertSize(CONNERS);
	SetIdxSize(CONNERS);

	float rad = glm::pi<float>() * 2 / CONNERS;

	for (uint i = 0; i < CONNERS; ++i)
	{
		GetVerts()[i].x = glm::cos(rad * i);
		GetVerts()[i].y = 0.0f;
		GetVerts()[i].z = glm::sin(rad * i);
		GetVerts()[i].w = 1.0f;

		GetIndxs()[i] = i;
	}

	_isBuilt = true;
}