#pragma once

#include "Global.h"
#include <map>
#include <vector>
#include "Terrain.h"
#include "Shader.h"
#include "Camera.h"

struct Octant;

typedef std::shared_ptr<Octant> OctantSptr;
typedef std::shared_ptr<point4> VertSptr;
typedef std::shared_ptr<uint> IndexSptr;

struct Octant
{
	uint vertNum; // number of trianles in this node
	int depth; // at what level 
	point3 center; // the center position of this node
	float radius; // it's a square radius
	std::vector<uint> indices;
	OctantSptr child[8]; // its children
};

class Octree
{
public:
	Octree();
	~Octree();
	void Build(point3 origin, float radius, int maxUnit, int maxDepth);
	void DebugDraw(Camera *cam, Shader *shader);
protected:
	void Generate(OctantSptr start, int depth);
private:
	OctantSptr _root;
	int _maxDepth;
	uint _maxVertsNum;
	int  _currentDepth;
	int _numOfOctants;
	enum{
		FRONT_UP_LEFT = 0,
		RRONT_UP_RIGHT,
		RRONT_DOWN_LEFT,
		RRONT_DOWN_RIGHT,
		BACK_UP_LEFT,
		BACK_UP_RIGHT,
		BACK_DOWN_LEFT,
		BACk_DOWN_RIGHT
	};
	//std::map<float, OctantSptr> _octree;
	//float == depth + 0.1 * one of the enum
private:
	//Draw Octree;
	GLuint _vao, _vbo;
	point3* _octreeVerts;
	void _InitOctreeDrawData();
	void _GenDrawData(OctantSptr node, uint &vertCount);
};

