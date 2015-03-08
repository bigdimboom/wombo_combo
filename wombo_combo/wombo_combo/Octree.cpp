#include "Octree.h"

void InitNodeChildren(OctantSptr oct)
{
	for (int i = 0; i < 8; ++i)
	{
		oct->child[i] = nullptr;
	}
}

bool IsInBox(OctantSptr octSptr, point3 point)
{
	float x1 = octSptr->center.x - octSptr->radius;
	float x2 = octSptr->center.x + octSptr->radius;
	float y1 = octSptr->center.y - octSptr->radius;
	float y2 = octSptr->center.y + octSptr->radius;
	float z1 = octSptr->center.z - octSptr->radius;
	float z2 = octSptr->center.z + octSptr->radius;

	if (point.x >= x1 && point.x <= x2 &&
		point.y >= y1 && point.y <= y2 &&
		point.z >= z1 && point.z <= z2)
	{
		return true;
	}

	return false;
}

static bool IsLeafNode(OctantSptr octSptr)
{
	return octSptr->child[0] == nullptr;
}

Octree::Octree(VertSptr verts, int vSize, IndexSptr idx, int idxSize)
	:_maxDepth(0),
	_maxVertsNum(0), _root(nullptr)
{
	_rawVerts = verts;
	_rawIndices = idx;
	_vertsSize = vSize;
	_indexSize = idxSize;
}


Octree::~Octree()
{
}

void Octree::Build(point3 origin, float radius, int maxPerUnit, int maxDepth)
{
	_maxDepth = maxDepth;
	_maxVertsNum = maxPerUnit;
	// init the Root
	_root = std::make_shared<Octant>();
	_root->depth = 0;
	_root->vertsNum = _indexSize;
	_root->center = origin;
	_root->radius = radius;
	_root->indices.assign(_rawIndices.get(), _rawIndices.get() + _indexSize);
	InitNodeChildren(_root);
	Generate(_root, _maxDepth);
}

void Octree::Generate(OctantSptr start, int depth)
{
	static int currentDepth = 0;

	if (_maxDepth - currentDepth <= 0 || 
		start->vertsNum <= _maxVertsNum)
	{
		return;
	}

	start->indices.clear();
	start->indices.shrink_to_fit();

	//Init 8 children nodes
	for (uint i = 0; i < 8; ++i)
	{
		start->child[i] = std::make_shared<Octant>();
		start->child[i]->vertsNum = 0;
		start->child[i]->radius = (start->radius / 2.0f);
		start->child[i]->depth = currentDepth + 1;
		start->child[i]->indices.reserve(_maxVertsNum);
		InitNodeChildren(start->child[i]);
		switch (i)
		{
		case FRONT_UP_LEFT:
			start->child[i]->center.x = start->center.x - start->child[i]->radius;
			start->child[i]->center.y = start->center.y + start->child[i]->radius;
			start->child[i]->center.z = start->center.z - start->child[i]->radius;
			break;
		case RRONT_UP_RIGHT:
			start->child[i]->center.x = start->center.x + start->child[i]->radius;
			start->child[i]->center.y = start->center.y + start->child[i]->radius;
			start->child[i]->center.z = start->center.z - start->child[i]->radius;
			break;
		case RRONT_DOWN_LEFT:
			start->child[i]->center.x = start->center.x - start->child[i]->radius;
			start->child[i]->center.y = start->center.y - start->child[i]->radius;
			start->child[i]->center.z = start->center.z - start->child[i]->radius;
			break;
		case RRONT_DOWN_RIGHT:
			start->child[i]->center.x = start->center.x + start->child[i]->radius;
			start->child[i]->center.y = start->center.y - start->child[i]->radius;
			start->child[i]->center.z = start->center.z - start->child[i]->radius;
			break;
		case BACK_UP_LEFT:
			start->child[i]->center.x = start->center.x - start->child[i]->radius;
			start->child[i]->center.y = start->center.y + start->child[i]->radius;
			start->child[i]->center.z = start->center.z + start->child[i]->radius;
			break;
		case BACK_UP_RIGHT:
			start->child[i]->center.x = start->center.x + start->child[i]->radius;
			start->child[i]->center.y = start->center.y + start->child[i]->radius;
			start->child[i]->center.z = start->center.z + start->child[i]->radius;
			break;
		case BACK_DOWN_LEFT:
			start->child[i]->center.x = start->center.x - start->child[i]->radius;
			start->child[i]->center.y = start->center.y - start->child[i]->radius;
			start->child[i]->center.z = start->center.z + start->child[i]->radius;
			break;
		case BACk_DOWN_RIGHT:
			start->child[i]->center.x = start->center.x + start->child[i]->radius;
			start->child[i]->center.y = start->center.y - start->child[i]->radius;
			start->child[i]->center.z = start->center.z + start->child[i]->radius;
			break;
		}
	}

	point3 pos;
	for (uint idx = 0; idx < _indexSize; ++idx)
	{
		pos = point3(_rawVerts.get()[_rawIndices.get()[idx]]);
		for (int i = 0; i < 8; ++i)
		{
			if (IsInBox(start->child[i], pos) /*&& IsLeafNode(start->child[i])*/)
			{
				start->child[i]->indices.push_back(_rawIndices.get()[idx]);
				++start->child[i]->vertsNum;
			}
		}
	}

	++currentDepth;
	
	for (uint i = 0; i < 8; ++i)
	{
		start->child[i]->indices.shrink_to_fit();
		Generate(start->child[i], currentDepth);
	}
}