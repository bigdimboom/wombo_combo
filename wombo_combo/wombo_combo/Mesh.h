#pragma once
#include "Global.h"
class Mesh
{
public:
	Mesh();
	~Mesh();
private:
	point4* _verts;
	point3* _normals;
	point2* _uvs;
};

