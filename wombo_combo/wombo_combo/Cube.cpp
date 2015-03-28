#include "Cube.h"


Cube::Cube()
{
	Init();
}


Cube::~Cube()
{
}

void Cube::Init()
{
	point4 verts[] = {
		point4(-1.0f, -1.0f, 1.0f, 1.0f),
		point4(1.0f, -1.0f, 1.0f, 1.0f),
		point4(1.0f, 1.0f, 1.0f, 1.0f),
		point4(-1.0f, 1.0f, 1.0f, 1.0f),
		point4(-1.0f, -1.0f, -1.0f, 1.0f),
		point4(1.0f, -1.0f, -1.0f, 1.0f),
		point4(1.0f, 1.0f, -1.0f, 1.0f),
		point4(-1.0f, 1.0f, -1.0f, 1.0f)
	};

	uint cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		3, 2, 6,
		6, 7, 3,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// left
		4, 0, 3,
		3, 7, 4,
		// right
		1, 5, 6,
		6, 2, 1,
	};

	this->SetVertSize(8);
	memcpy(this->GetVerts(), verts, sizeof(point4) * 8);
	SetIdxSize(6 * 6);
	memcpy(this->GetIndxs(), cube_elements, sizeof(uint) * 36);
}