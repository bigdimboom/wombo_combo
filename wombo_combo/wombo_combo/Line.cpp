#include "Line.h"


Line::Line(const point4& from, const point4& to)
{
	Init(from, to);
}


Line::~Line()
{
}


void Line::Init(const point4& from, const point4& to)
{
	point4 verts[] = {
		from,
		to
	};

	uint indices[] = {
		0,1
	};

	this->SetVertSize(2);
	memcpy(this->GetVerts(), verts, sizeof(point4) * 2);
	SetIdxSize(2);
	memcpy(this->GetIndxs(), indices, sizeof(uint) * 2);
}