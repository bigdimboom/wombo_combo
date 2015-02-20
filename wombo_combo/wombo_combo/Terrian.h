#pragma once

#include "IRenderable.h"
#include "Global.h"
#include <SDL_image.h>

//Using templete pattern

class Terrian
{
public:
	Terrian(int length=1024, int width=1024);
	~Terrian();

	void GenHightMap(const char* hightmap);
	inline void SetWeight(float weight){ _weight = weight; }
	point4* GetPureTerrian() const { return _terrian; }
	//TODO:
	//point3* GetNormals() const;
	//point2* GetUVCoords() const;

private:
	point4* _terrian;
	point3* _normals;
	point2* _textureCoords;
	int _length, _width;
	float _weight;
};

