#pragma once

#include "Global.h"
#include <map>
#include <vector>
#include "Terrain.h"
#include "Shader.h"
#include "Camera.h"

struct Octant;

typedef Octant* OctantPtr;
typedef std::string PosStr;
typedef std::map<PosStr, OctantPtr >::iterator OctreeIterator;

struct Octant
{
	uint vertSize; // number of trianles in this node
	point3 center; // the center position of this node
	float radius; // it's a square radius
	std::vector<uint> indices;
	OctantPtr child[8]; // its children
};

class Octree
{
public:
	Octree();
	~Octree();
	void BindMesh(Mesh* mesh, point3 origin, float radius);
	void Build(int maxUnit, int maxDepth);
	void Destory();
	void DebugDraw(Camera *cam, Shader *shader);
	static bool IsInBox(OctantPtr octPtr, point3 point);
	static bool IsLeafNode(OctantPtr octPtr);
	OctantPtr GetRoot();
	OctantPtr GetOcant(point3 pos);
protected:
	void Generate(OctantPtr start, int depth);
	void InitOctreeDrawData();
private:
	Mesh* _rawMesh;
	OctantPtr _root;
	int _maxDepth;
	uint _maxVSizePerNode;
	std::map<PosStr,OctantPtr> _octree;

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
	//float == depth + 0.1 * one of the enum
	static void _InitChildrenCenter(OctantPtr start, int i);
private:
	//Draw Octree;
	GLuint _vao, _vbo;
	point3* _octreeVerts;
	void _GenDrawData(OctantPtr nodePtr, uint &vertCount);
};

