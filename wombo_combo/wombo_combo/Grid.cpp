#include "Grid.h"


Grid::Grid(int sizex, int sizez)
	:_isBuilt(false)
{
	Init(sizex, sizez);
}

Grid::Grid(int size)
{
	Init(size, size);
}


Grid::~Grid()
{
}

void Grid::Init(int sizex, int sizez)
{
	assert(_isBuilt != true);
	int mysize = sizex * sizez;
	SetVertSize((uint)mysize);

	//only red for heightmap
	for (int z = 0; z < (int)sizez; ++z)
	{
		for (int x = 0; x < (int)sizex; ++x)
		{
			GetVerts()[z * sizex + x] =
				point4(((float)x - (float)(sizex / 2.0f)),
				0.0f,
				((float)z - (float)(sizez / 2.0f)), 1.0f);
			//printf("%d\n", cccc);
		}
	}

	int indexSize = (sizex - 1) * (sizez - 1) * 8;
	SetIdxSize(indexSize);
	int i = 0;

	for (int y = 0; y < (int)sizez - 1; ++y)
	{
		for (int x = 0; x < (int)sizex - 1; ++x)
		{
			GetIndxs()[i] = y*(sizex)+x; // x 
			GetIndxs()[i + 1] = y*(sizex)+x + 1; // x + 1

			GetIndxs()[i + 2] = y*(sizex)+x + 1; // x + 1
			GetIndxs()[i + 3] = y*(sizex)+x + 1 + sizex; // x + 1 + _width

			GetIndxs()[i + 4] = y*(sizex)+x + 1 + sizex; // x + 1 + _width
			GetIndxs()[i + 5] = y*(sizex)+x + sizex;

			GetIndxs()[i + 6] = y*(sizex)+x + sizex;
			GetIndxs()[i + 7] = y*(sizex)+x; // x 

			i += 8;
		}
	}
}
