#pragma once
#include "Mesh.h"
class WorldPlane :
	public Mesh
{
public:
	WorldPlane(int size);
	~WorldPlane();
	void Init(int size);
private:
	bool _isBuilt;
};

