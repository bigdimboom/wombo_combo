#include "MeshRender.h"


MeshRender::MeshRender()
{

}


MeshRender::~MeshRender()
{
	if (_mesh != nullptr)
	{
		_mesh->ClearAll();
	}
}

void MeshRender::Init(Mesh* mesh, GLenum drawMode)
{
	assert(mesh != nullptr);
	_mesh = mesh;
	_drawMode = drawMode;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _mesh->GetVSizeInBytes(), _mesh->GetVerts(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIdxSizeInBytes(), _mesh->GetIndxs(), GL_DYNAMIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	_mesh->ClearVerts();
}

void MeshRender::Render(Camera* cam, Shader* shader, color4& color, matrix4& model, float lineWidth, bool isDepthEnabled)
{
	assert(_mesh != nullptr);
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
	GLint colorLoc = glGetUniformLocation(shader->GetID(), "color");

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform4fv(colorLoc, 1, glm::value_ptr(color));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	if (!isDepthEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(lineWidth);
	glDrawElements(_drawMode, _mesh->GetIdxSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	//Recover from old states
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0f);
	glEnable(GL_CULL_FACE);

	if (!isDepthEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}


	glBindVertexArray(0);
}