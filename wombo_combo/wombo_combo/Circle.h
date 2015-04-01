#pragma once
#include "Mesh.h"
class Circle :
	public Mesh
{
public:
	Circle(const point4& center,
		const point3& planeNormal,
		float radius);
	~Circle();
	void Init(const point4& center,
		const point3& planeNormal,
		float radius);
private:
	bool _isBuilt;
};

