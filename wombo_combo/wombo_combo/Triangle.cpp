#include "Triangle.h"


Triangle::Triangle(const point4& v0,
	const point4& v1,
	const point4& v2)
	:_isBuilt(false)
{
	Init(v0, v1, v2);
}


Triangle::~Triangle()
{
}

void Triangle::Init(const point4& v0,
	const point4& v1,
	const point4& v2)
{
	assert(_isBuilt != true);
	point4 verts[] = {
		v0,v1,v2
	};

	uint indices[] = {
		0, 1, 2
	};

	this->SetVertSize(3);
	memcpy(this->GetVerts(), verts, sizeof(point4) * 3);
	SetIdxSize(3);
	memcpy(this->GetIndxs(), indices, sizeof(uint) * 3);

	_isBuilt = true;
}