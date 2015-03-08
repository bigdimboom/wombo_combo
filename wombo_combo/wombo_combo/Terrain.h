#pragma once

#include "IRenderable.h"
#include "Global.h"
#include <SDL_image.h>
#include <fstream>
#include <iomanip>
#include "Mesh.h"

//Using templete pattern
//user could override terrian-gen methods 
//in order to change the behavior of 
//"GenTerrian(const char*, bool, bool)"

#define NORMAL_MAP_FILE "NORMAL.Data"

class Terrain
{
public:
	Terrain(int length = 1024, int width = 1024);
	~Terrain();

	void GenTerrian(const char* hightmap, bool genNormals = false, bool genUVs = false);
	inline void SetWeight(float weight){ _weight = weight; }
	inline Mesh& GetMesh() { return _mesh; }
protected:
	virtual void GenHeightMap(const char* hightmap);
	virtual void GenIndicesArray();
	virtual void GenNormals();
	virtual void GenTextureCoords();
private:
	Mesh _mesh;
	uint _length, _width;
	float _weight;
};
