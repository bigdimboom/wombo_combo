#pragma once
#include "Mesh.h"
class Grid :
	public Mesh
{
public:
	Grid(int size);
	Grid(int sizex, int sizez);
	~Grid();
	void Init(int sizex, int sizez);
private:
	bool _isBuilt;
};

