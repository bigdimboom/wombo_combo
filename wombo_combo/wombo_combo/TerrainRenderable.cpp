#include "TerrainRenderable.h"


TerrainRenderable::TerrainRenderable()
{
	_terrain = new Terrain(512, 512);
	_isCull = false;
}

TerrainRenderable::TerrainRenderable(int width, int height)
{
	_terrain = new Terrain(width, height);
	_isCull = false;
}

TerrainRenderable::TerrainRenderable(Terrain* inst)
{
	assert(inst != nullptr);
	_terrain = inst;
	_isOutsideInstance = true;
	_isCull = false;
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
		_terrain->GetPosition());

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
		glUniform1i( glGetUniformLocation( shader->GetID(), _textures[i].second.second), i );
	}

	glDrawElements(GL_TRIANGLES, _terrain->GetMesh().GetIdxSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);
}


void TerrainRenderable::AttachTexture(Path path, Name name)
{
	_textures.push_back ( 
		std::make_pair (
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