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

Octree::Octree()
{
}


Octree::~Octree()
{
}

void Octree::Build(point3 origin, float radius, int maxPerUnit, int maxDepth)
{

}

void Octree::Generate(OctantSptr start, int depth)
{
	_currentDepth = 0;

	if (_maxDepth - _currentDepth == 0 ||
		start->vertNum <= _maxVertsNum)
	{
		return;
	}

	//Init 8 children nodes
	for (uint i = 0; i < 8; ++i)
	{
		start->child[i] = OctantSptr(new Octant);
		start->child[i]->vertNum = 0;
		start->child[i]->radius = (start->radius / 2.0f);
		start->child[i]->depth = _currentDepth + 1;
		start->child[i]->indices.reserve(start->vertNum);
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
	for (uint idx = 0; idx < start->vertNum; ++idx)
	{
		pos = point3(_rawVerts.get()[idx]);
		for (int i = 0; i < 8; ++i)
		{
			if (IsInBox(start->child[i], pos) && IsLeafNode(start->child[i]))
			{
				++start->child[i]->vertNum;
			}
		}
	}

	start->indices.clear();
	start->indices.shrink_to_fit();

	++_currentDepth;

	for (uint i = 0; i < 8; ++i)
	{
		Generate(start->child[i], _currentDepth);
	}
}

void Octree::_InitOctreeDrawData()
{
	uint vertSize = sizeof(point3) * _numOfOctants * 24;

	_octreeVerts = new point3[_numOfOctants * 24];

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

void Octree::_GenDrawData(OctantSptr node, uint &vertCount)
{
	if (IsLeafNode(node)){
		return;
	}

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

	glDrawArrays(GL_LINES, 0, _numOfOctants * 24);
	glBindVertexArray(0);
}