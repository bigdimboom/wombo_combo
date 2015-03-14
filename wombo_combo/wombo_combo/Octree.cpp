#include "Octree.h"

void InitNodeChildren(OctantPtr octPtr)
{
	for (int i = 0; i < 8; ++i)
	{
		octPtr->child[i] = nullptr;
	}
}

void Octree::_InitChildrenCenter(OctantPtr start, int i)
{
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

Octree::Octree()
	:_rawMesh(nullptr),
	_root(nullptr),
	_maxDepth(0),
	_maxVSizePerNode(0)
{
}


Octree::~Octree()
{
	Destory();
}


void Octree::BindMesh(Mesh* mesh, point3 origin, float radius)
{
	_rawMesh = mesh;
	_root = new Octant;
	_root->vertSize = _rawMesh->GetIdxSize();
	_root->center = origin;
	_root->radius = radius;
	_root->indices.assign(_rawMesh->GetIndxs(),
		_rawMesh->GetIndxs() + _rawMesh->GetIdxSize());
	InitNodeChildren(_root);
	_octree[glm::to_string(_root->center)] = _root;
}


void Octree::Build(int maxSizePerNode, int maxDepth)
{
	assert(_rawMesh != nullptr);
	_maxDepth = maxDepth;
	_maxVSizePerNode = maxSizePerNode;
	Generate(_root, _maxDepth);
	InitOctreeDrawData();
}

void Octree::Destory()
{
	_rawMesh = nullptr;
	OctreeIterator itr;
	for (itr = _octree.begin(); itr != _octree.end(); ++itr)
	{
		delete itr->second;
	}
	_octree.clear();
}

void Octree::Generate(OctantPtr start, int depth)
{
	if (depth == 0 ||
		start->vertSize <= _maxVSizePerNode)
	{
		return;
	}

	//Init 8 children nodes
	for (uint i = 0; i < 8; ++i)
	{
		start->child[i] = new Octant;
		start->child[i]->vertSize = 0;
		start->child[i]->radius = (start->radius / 2.0f);
		//Be cautious of the next two
		InitNodeChildren(start->child[i]);
		_InitChildrenCenter(start, i); //init its children's center//This is corrent for future reference
		_octree[glm::to_string(start->child[i]->center)] = start->child[i];
	}

	point3 pos;
	for (uint idx = 0; idx < start->vertSize; ++idx)
	{
		pos = point3(_rawMesh->GetVerts()[start->indices[idx]]);
		for (int i = 0; i < 8; ++i)
		{
			if (IsInBox(start->child[i], pos) && IsLeafNode(start->child[i]))
			{
				start->child[i]->indices.push_back(start->indices[idx]);
				++start->child[i]->vertSize;

				if (start->child[i]->vertSize % 3 != 0){
					start->child[i]->indices.push_back(start->indices[idx + 1]);
					++start->child[i]->vertSize;
				}

				if (start->child[i]->vertSize % 3 != 0){
					start->child[i]->indices.push_back(start->indices[idx + 2]);
					++start->child[i]->vertSize;
				}
			}
		}
	}

	start->indices.clear();
	start->indices.shrink_to_fit();

	for (uint i = 0; i < 8; ++i)
	{
		Generate(start->child[i], depth - 1);
	}
}


void Octree::InitOctreeDrawData()
{
	uint vertSize = sizeof(point3) * _octree.size() * 24;

	_octreeVerts = new point3[_octree.size() * 24];

	uint vertsCount = 0;

	_GenDrawData(_root, vertsCount);

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize, _octreeVerts, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glBindVertexArray(0); // Unbind VAO

	delete[] _octreeVerts;
}


void Octree::_GenDrawData(OctantPtr node, uint &vertCount)
{
	point3 ftop_left;
	ftop_left.x = node->center.x - node->radius;
	ftop_left.y = node->center.y + node->radius;
	ftop_left.z = node->center.z - node->radius;

	point3 ftop_right;
	ftop_right.x = node->center.x + node->radius;
	ftop_right.y = node->center.y + node->radius;
	ftop_right.z = node->center.z - node->radius;

	point3 fdown_right;
	fdown_right.x = node->center.x + node->radius;
	fdown_right.y = node->center.y - node->radius;
	fdown_right.z = node->center.z - node->radius;

	point3 fdown_left;
	fdown_left.x = node->center.x - node->radius;
	fdown_left.y = node->center.y - node->radius;
	fdown_left.z = node->center.z - node->radius;

	///////////////////////////////////////////////
	point3 btop_left;
	btop_left.x = node->center.x - node->radius;
	btop_left.y = node->center.y + node->radius;
	btop_left.z = node->center.z + node->radius;

	point3 btop_right;
	btop_right.x = node->center.x + node->radius;
	btop_right.y = node->center.y + node->radius;
	btop_right.z = node->center.z + node->radius;

	point3 bdown_right;
	bdown_right.x = node->center.x + node->radius;
	bdown_right.y = node->center.y - node->radius;
	bdown_right.z = node->center.z + node->radius;

	point3 bdown_left;
	bdown_left.x = node->center.x - node->radius;
	bdown_left.y = node->center.y - node->radius;
	bdown_left.z = node->center.z + node->radius;

	_octreeVerts[vertCount++] = ftop_left;
	_octreeVerts[vertCount++] = ftop_right;
	_octreeVerts[vertCount++] = ftop_right;
	_octreeVerts[vertCount++] = fdown_right;
	_octreeVerts[vertCount++] = fdown_right;
	_octreeVerts[vertCount++] = fdown_left;
	_octreeVerts[vertCount++] = fdown_left;
	_octreeVerts[vertCount++] = ftop_left;

	_octreeVerts[vertCount++] = btop_left;
	_octreeVerts[vertCount++] = btop_right;
	_octreeVerts[vertCount++] = btop_right;
	_octreeVerts[vertCount++] = bdown_right;
	_octreeVerts[vertCount++] = bdown_right;
	_octreeVerts[vertCount++] = bdown_left;
	_octreeVerts[vertCount++] = bdown_left;
	_octreeVerts[vertCount++] = btop_left;

	_octreeVerts[vertCount++] = ftop_left;
	_octreeVerts[vertCount++] = btop_left;
	_octreeVerts[vertCount++] = ftop_right;
	_octreeVerts[vertCount++] = btop_right;

	_octreeVerts[vertCount++] = fdown_right;
	_octreeVerts[vertCount++] = bdown_right;
	_octreeVerts[vertCount++] = fdown_left;
	_octreeVerts[vertCount++] = bdown_left;

	if (IsLeafNode(node)){
		return;
	}

	for (uint i = 0; i < 8; ++i)
	{
		_GenDrawData(node->child[i], vertCount);
	}
}


void Octree::DebugDraw(Camera *cam, Shader *shader)
{
	assert(cam != nullptr && shader != nullptr);
	shader->Use();
	glBindVertexArray(_vao);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->GetID(), "model");
	GLint viewLoc = glGetUniformLocation(shader->GetID(), "view");
	GLint projLoc = glGetUniformLocation(shader->GetID(), "projection");

	glm::mat4 view;
	view = *(cam->GetViewMatrix());
	glm::mat4 projection = *(cam->GetProjMatrix());

	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix4(1.0f)));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_LINES, 0, _octree.size() * 24);
	glBindVertexArray(0);
}


bool Octree::IsInBox(OctantPtr octPtr, point3 point)
{
	float x1 = octPtr->center.x - octPtr->radius;
	float x2 = octPtr->center.x + octPtr->radius;
	float y1 = octPtr->center.y - octPtr->radius;
	float y2 = octPtr->center.y + octPtr->radius;
	float z1 = octPtr->center.z - octPtr->radius;
	float z2 = octPtr->center.z + octPtr->radius;

	if (point.x >= x1 && point.x <= x2 &&
		point.y >= y1 && point.y <= y2 &&
		point.z >= z1 && point.z <= z2)
	{
		return true;
	}

	return false;
}

bool Octree::IsLeafNode(OctantPtr octPtr)
{
	if (octPtr->child[0] == nullptr)
	{
		return true;
	}

	return false;
}

OctantPtr Octree::GetRoot()
{
	assert(_octree.size() != 0);
	return _root;
}

OctantPtr Octree::GetOcant(point3 pos)
{
	assert(_octree.size() != 0);
	OctreeIterator itr = _octree.find(glm::to_string(pos));
	return itr->second;
}