#pragma once
#include "Mesh.h"
class Cube :
	public Mesh
{
public:
	Cube();
	~Cube();
	void Init();
private:
	bool _isBuilt;
};

