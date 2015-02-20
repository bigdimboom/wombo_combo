#pragma once

#include "IRenderable.h"
#include "Global.h"
#include <SDL_image.h>

//Using templete pattern
//user could override terrian-gen methods 
//in order to change the behavior of 
//"GenTerrian(const char*, bool, bool)"

class Terrian
{
public:
	Terrian(int length=1024, int width=1024);
	~Terrian();

	void GenTerrian(const char* hightmap, bool genNormals = false, bool genUVs=false);
	inline void SetWeight(float weight){ _weight = weight; }
	inline point4* GetPureTerrian() const { return _terrian; }
	inline uint* GetIndices() const { return _indices; }
	inline uint GetSize() const { return _width * _length; }
	inline uint GetIndicesNum() const { return _index_size; }
protected:
	virtual void GenHeightMap(const char* hightmap);
	virtual void GenIndicesArray();
	//TODO:
	//point3* GetNormals() const;
	//point2* GetUVCoords() const;
private:
	point4* _terrian;
	point3* _normals;
	uint* _indices;
	point2* _textureCoords;
	int _length, _width;
	GLuint _index_size;
	float _weight;
};

