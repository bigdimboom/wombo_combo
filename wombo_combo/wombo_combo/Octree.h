#pragma once

#include "Global.h"
#include <map>
#include <vector>
#include "Terrain.h"

struct Octant;

typedef std::shared_ptr<Octant> OctantSptr;
typedef std::shared_ptr<point4> VertSptr;
typedef std::shared_ptr<uint> IndexSptr;

struct Octant
{
	int vertsNum; // number of trianles in this node
	int depth; // at what level 
	point3 center; // the center position of this node
	float radius; // it's a square radius
	std::vector<uint> indices;
	OctantSptr child[8]; // its children
};

class Octree
{
public:
	Octree(VertSptr verts, int vSize, IndexSptr idx, int idxSize);
	~Octree();
	void Build(point3 origin, float radius, int maxUnit, int maxDepth);
protected:
	void Generate(OctantSptr start, int depth);
private:
	OctantSptr _root;
	int _maxDepth;
	int _maxVertsNum;
	int  _currentDepth;
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
	VertSptr _rawVerts;
	uint _vertsSize;
	IndexSptr _rawIndices;
	uint _indexSize;
};

