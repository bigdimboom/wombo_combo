#pragma once
#include "Mesh.h"
#include <vector>

#define CONNERS 360

class Circle :
	public Mesh
{
public:
	Circle();
	~Circle();
	void Init();
private:
	bool _isBuilt;
};

