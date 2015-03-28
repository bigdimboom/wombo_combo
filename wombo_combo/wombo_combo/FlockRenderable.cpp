#include "FlockRenderable.h"


FlockRenderable::FlockRenderable()
	:Flock(), _isInited(false)
{
}


FlockRenderable::~FlockRenderable()
{

}

void FlockRenderable::Render(Camera* cam, point3* lightPos, Shader* shader)
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
	GLint colorLoc = glGetUniformLocation(shader->GetID(), "color");

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	matrix4 model = matrix4(1.0f);
	point4 color = point4(0.2, 0.5, 0.8, 1.0);

	for (uint i = 0; i < flock.size(); ++i)
	{
		model = glm::translate(matrix4(1.0), flock[i]->GetPosition());
		glUniform4fv(colorLoc, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, flock[i]->GetMesh()->GetIdxSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	}
	glBindVertexArray(0);
}

void FlockRenderable::Init()
{
	assert(_isInited == false);
	_isInited = true;

	meshes["test"] = Mesh_SPtr(new Cube);

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, meshes["test"]->GetVSizeInBytes(), meshes["test"]->GetVerts(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes["test"]->GetIdxSizeInBytes(), meshes["test"]->GetIndxs(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	int boidSize = 50;
	point3 pos, vel;

	pos = point3(0.0, 20.0f, 0.0f);
	for (int i = 0; i < boidSize; ++i)
	{
		vel = point3(glm::linearRand(-0.1f, 0.1f), glm::linearRand(-0.1f, 0.1f), glm::linearRand(-0.1f, 0.1f));
		pos = point3(glm::linearRand(-30.0f, 30.0f), 30.0f, glm::linearRand(-30.0f, 30.0f));
		flock.push_back(new Boid(pos,vel));
		flock[i]->SetRadius(5.0f);
		flock[i]->SetMesh(meshes["test"].get());
	}
}