#include "TerrainRenderable.h"


TerrainRenderable::TerrainRenderable()
{
	_terrain = new Terrain(512, 512);
	_isCull = true;
	_isDebug = false;
}

TerrainRenderable::TerrainRenderable(int width, int height)
{
	_terrain = new Terrain(width, height);
	_isCull = true;
	_isDebug = false;
}

TerrainRenderable::TerrainRenderable(Terrain* inst)
{
	assert(inst != nullptr);
	_terrain = inst;
	_isOutsideInstance = true;
	_isCull = true;
	_isDebug = false;
}


TerrainRenderable::~TerrainRenderable()
{
	if (_isOutsideInstance == false)
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
		delete _terrain;
	}
}

void TerrainRenderable::Init()
{
	_InitMembers();

	_terrain->GenTerrian("Assets/Terrain/heightmap.jpeg", true, true);

	//load textures
	if (_textures.size() == 0)
	{
		std::cout << "Warning: no textures loaded.\n";
	}
	else
	{
		for (uint i = 0; i < _textures.size(); ++i)
		{
			_textures[i].first->Load(_textures[i].second.first, true);
		}
	}

	_octree.BindMesh(&_terrain->GetMesh(),
		_terrain->GetPosition(), (float)_terrain->GetMaxSize() / 2.0f);
	_octree.Build(NUM_TRIANGLES, LAYERS);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _terrain->GetMesh().GetVSizeInBytes() +
		_terrain->GetMesh().GetNormSizeInBytes() +
		_terrain->GetMesh().GetUVSizeInBytes(), nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, _terrain->GetMesh().GetVSizeInBytes(), _terrain->GetMesh().GetVerts());
	glBufferSubData(GL_ARRAY_BUFFER, _terrain->GetMesh().GetVSizeInBytes(), _terrain->GetMesh().GetNormSizeInBytes(), _terrain->GetMesh().GetNorms());

	glBufferSubData(GL_ARRAY_BUFFER, _terrain->GetMesh().GetVSizeInBytes() +
		_terrain->GetMesh().GetNormSizeInBytes(), _terrain->GetMesh().GetUVSizeInBytes(), _terrain->GetMesh().GetUVs());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _terrain->GetMesh().GetIdxSizeInBytes(), _terrain->GetMesh().GetIndxs(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(_terrain->GetMesh().GetVSizeInBytes()));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(_terrain->GetMesh().GetVSizeInBytes() + _terrain->GetMesh().GetNormSizeInBytes()));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void TerrainRenderable::Render(Camera* cam, point3* lightPos, Shader* shader)
{
	assert(cam != nullptr && shader != nullptr);
	shader->Use();
	cam->Update();

	glBindVertexArray(_vao);
	// Create camera transformation
	glm::mat4 view;
	view = *(cam->GetViewMatrix());
	glm::mat4 projection;
	projection = *(cam->GetProjMatrix());
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->GetID(), "model");
	GLint viewLoc = glGetUniformLocation(shader->GetID(), "view");
	GLint projLoc = glGetUniformLocation(shader->GetID(), "projection");

	GLint lightPosLoc = glGetUniformLocation(shader->GetID(), "lightPosition");
	glUniform3f(lightPosLoc, lightPos->x, lightPos->y, lightPos->z);
	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix4(1.0f)));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	for (uint i = 0; i < _textures.size(); ++i)
	{
		_textures[i].first->Bind((int)i);
		glUniform1i(glGetUniformLocation(shader->GetID(), _textures[i].second.second), i);
	}

	if (_isCull)
	{
		_frustum.Set(cam);

		_idx.clear();
		_Cull(_octree.GetRoot());
		_isCull = false;
	}

	if (_isDebug)
	{
		_isCull = true;
		_Cull(_octree.GetRoot());
		_isCull = false;
		_isDebug = false;
	}

	for (uint i = 0; i < _idx.size(); ++i)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * _idx[i]->size(), &_idx[i]->at(0), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, (GLsizei)_idx[i]->size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	}

	glBindVertexArray(0);
}


void TerrainRenderable::AttachTexture(Path path, Name name)
{
	_textures.push_back(
		std::make_pair(
		new Texture(), std::make_pair(path, name)
		)
		);
}


void TerrainRenderable::_InitMembers()
{
	_isOutsideInstance = false;
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

void TerrainRenderable::_Cull(OctantPtr ptr)
{

	if (ptr == nullptr || !_isCull || !_frustum.IsCubeInside(ptr->center, ptr->radius))
	{
		return;
	}

	if (_octree.IsLeafNode(ptr) && ptr->indices.size() != 0)
	{

		std::cout << ptr->indices.size() << std::endl;

		_idx.push_back(&ptr->indices);

		if (_isDebug)
		{
			point4 v[8];
			for (int i = 0; i < 8; ++i)
			{
				v[i] = point4(Plane::GetPointFromCube(ptr->center, ptr->radius, i), 1.0f);
			}

			DebugDrawManager::getInstance().
				AddFrustum(v[0], v[1],
				v[3], v[2],
				v[4], v[5],
				v[7], v[6], color4(0.0f, 1.0f, 0.5, 1.0f), 1000.0f, 1.0f, false);
		}
	}

	for (uint i = 0; i < 8; ++i)
	{
		_Cull(ptr->child[i]);
	}
}