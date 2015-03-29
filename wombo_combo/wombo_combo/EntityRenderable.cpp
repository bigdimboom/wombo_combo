#include "EntityRenderable.h"


EntityRenderable::EntityRenderable()
	:Boid()
{
	_scaleModel = matrix4(1.0);
}


EntityRenderable::~EntityRenderable()
{
	GetMesh()->ClearAll();
}

void EntityRenderable::Scale(float scale)
{
	_scaleModel = glm::scale(matrix4(1.0), point3(scale,scale,scale));
}

void EntityRenderable::Init(Mesh* mesh)
{
	assert(mesh != nullptr);
	SetMesh(mesh);

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, GetMesh()->GetVSizeInBytes(), GetMesh()->GetVerts(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetMesh()->GetIdxSizeInBytes(), GetMesh()->GetIndxs(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO
}

void EntityRenderable::Render(Camera* cam, point3* lightPos, Shader* shader)
{
	assert(GetMesh() != nullptr);
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

	matrix4 model = matrix4(1.0f);
	point4 color = point4(0.1, 0.2, 0.8, 1.0);

	model = glm::translate(matrix4(1.0), GetPosition()) * _scaleModel;
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glDrawElements(GL_TRIANGLES, GetMesh()->GetIdxSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}