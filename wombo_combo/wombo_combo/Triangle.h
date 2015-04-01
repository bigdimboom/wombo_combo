#pragma once
#include "Mesh.h"
class Triangle
	:public Mesh
{
public:
	Triangle(const point4& v0,
		const point4& v1,
		const point4& v2);
	~Triangle();

	void Init(const point4& v0,
		const point4& v1,
		const point4& v2);

private:
	bool _isBuilt;
};

