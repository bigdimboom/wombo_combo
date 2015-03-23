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

	point4 verts[] = {
		point4(-1.0f, -1.0f, 1.0f, 1.0f),
		point4(1.0f, -1.0f, 1.0f, 1.0f),
		point4(1.0f, 1.0f, 1.0f, 1.0f),
		point4(-1.0f, 1.0f, 1.0f, 1.0f),
		point4(-1.0f, -1.0f, -1.0f, 1.0f),
		point4(1.0f, -1.0f, -1.0f, 1.0f),
		point4(1.0f, 1.0f, -1.0f, 1.0f),
		point4(-1.0f, 1.0f, -1.0f, 1.0f)
	};

	uint cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		3, 2, 6,
		6, 7, 3,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// left
		4, 0, 3,
		3, 7, 4,
		// right
		1, 5, 6,
		6, 2, 1,
	};

	meshes["test"] = Mesh();
	meshes["test"].SetVertSize(8);
	memcpy(meshes["test"].GetVerts(), verts, sizeof(point4) * 8);
	meshes["test"].SetIdxSize(6 * 6);
	memcpy(meshes["test"].GetIndxs(), cube_elements, sizeof(uint) * 36);

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, meshes["test"].GetVSizeInBytes(), meshes["test"].GetVerts(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes["test"].GetIdxSizeInBytes(), meshes["test"].GetIndxs(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	int boidSize = 100;
	point3 pos, vel;

	pos = point3(0.0, 20.0f, 0.0f);
	for (int i = 0; i < boidSize; ++i)
	{
		vel = point3(glm::linearRand(-0.5f, 0.5f), glm::linearRand(0.1f, 0.2f), glm::linearRand(-0.5f, 0.5f));
		pos = point3(glm::linearRand(-30.0f, 30.0f), 30.0f, glm::linearRand(-30.0f, 30.0f));
		flock.push_back(new Boid(pos, vel));
		flock[i]->SetRadius(4.0f);
		flock[i]->SetMesh(&meshes["test"]);
	}
}