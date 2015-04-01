#include "WorldPlane.h"


WorldPlane::WorldPlane(int size)
	:_isBuilt(false)
{
	Init(size);
}


WorldPlane::~WorldPlane()
{
}

void WorldPlane::Init(int size)
{
	assert(_isBuilt != true);
	int mysize = size * size;
	SetVertSize((uint)mysize);

	//only red for heightmap
	for (int z = 0; z < (int)size; ++z)
	{
		for (int x = 0; x < (int)size; ++x)
		{
			GetVerts()[z * size + x] =
				point4(((float)x - (float)(size / 2.0f)),
				0.0f,
				((float)z - (float)(size / 2.0f)), 1.0f);
			//printf("%d\n", cccc);
		}
	}

	int indexSize = (size - 1) * (size - 1) * 8;
	SetIdxSize(indexSize);
	int i = 0;

	for (int y = 0; y < (int)size - 1; ++y)
	{
		for (int x = 0; x < (int)size - 1; ++x)
		{
			GetIndxs()[i] = y*(size)+x; // x 
			GetIndxs()[i + 1] = y*(size)+x + 1; // x + 1

			GetIndxs()[i + 2] = y*(size)+x + 1; // x + 1
			GetIndxs()[i + 3] = y*(size)+x + 1 + size; // x + 1 + _width

			GetIndxs()[i + 4] = y*(size)+x + 1 + size; // x + 1 + _width
			GetIndxs()[i + 5] = y*(size)+x + size;

			GetIndxs()[i + 6] = y*(size)+x + size;
			GetIndxs()[i + 7] = y*(size)+x; // x 

			i += 8;
		}
	}
}
