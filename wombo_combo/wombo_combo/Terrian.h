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
	inline std::shared_ptr<point4> GetPureTerrian() const { return _grids; }
	inline std::shared_ptr<uint> GetIndices() const { return _indices; }
	std::shared_ptr<normal3> GetNormals() const { return _normals; }
	inline uint GetSize() const { return _width * _length; }
	inline uint GetIndicesNum() const { return _index_size; }
protected:
	virtual void GenHeightMap(const char* hightmap);
	virtual void GenIndicesArray();
	virtual void GenNormals();
	//TODO:
	//point2* GetUVCoords() const;
private:
	std::shared_ptr<point4> _grids;
	std::shared_ptr<normal3> _normals;
	std::shared_ptr<uint> _indices;
	std::shared_ptr<point2> _textureCoords;
	uint _length, _width;
	GLuint _index_size;
	float _weight;
};
