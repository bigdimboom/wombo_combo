#include "ParticleEffect.h"


ParticleEffect::ParticleEffect()
{
}


ParticleEffect::~ParticleEffect()
{
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		delete _particleBox.at(i);
	}
}


void ParticleEffect::Init()
{
	_InitParticlesData();
	_InitDrawData();
}


void ParticleEffect::Update(float dTime)
{

	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if (!_particleBox.at(i)->Update(dTime))
		{
			delete _particleBox.at(i);
			_particleBox.at(i) = _particleFactory.CreateSingleParticle(
				color4(glm::linearRand(0.5f, 1.0f),
				glm::linearRand(0.1f, 0.7f),
				glm::linearRand(0.5f, 0.7f), 1.0f), 
				////
				glm::linearRand(1.5f, 2.0f));
			_InitDrawData();
		}
	}
}

void ParticleEffect::Render(Shader* shader, Camera* cam)
{
	assert(shader && cam);

	shader->Use();
	cam->Update();

	glBindVertexArray(_vao);
	// Create camera transformation
	glm::mat4 model(1.0f);
	glm::mat4 view;
	view = *(cam->GetViewMatrix());
	glm::mat4 projection;
	projection = *(cam->GetProjMatrix());
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->GetID(), "model");
	GLint viewLoc = glGetUniformLocation(shader->GetID(), "view");
	GLint projLoc = glGetUniformLocation(shader->GetID(), "projection");

	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
}


void ParticleEffect::_InitParticlesData()
{
	_particleFactory.SetProperties(point3(10.0f, 25.0f, 10.5f),
		point3(10.0f, 10.0f, 10.0f), point3(50.0f, 50.0f, 50.0f),
		point3(0.0f, -9.8f, 0.0f), color4(0.9, 0.2, 0.6, 1.0),
		20.0f, 30.0f,
		1.0f);

	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		Particle* p = _particleFactory.CreateSingleParticle(
			color4(glm::linearRand(0.5f, 1.0f),
			glm::linearRand(0.1f, 0.7f),
			glm::linearRand(0.5f, 0.7f), 1.0f), 
			/////
			glm::linearRand(2.0f, 3.0f));
		_particleBox.push_back(p);
	}

	glGenBuffers(1, &_vbo);
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES *
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),
		NULL, GL_STREAM_DRAW);
}



//point3 position; 0
//point3 velocity; 1
//point3 acceleration; 2
//point4 color; 3
//float life; 4
//float size; 5


void ParticleEffect::_InitDrawData()
{
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES *
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),
		NULL, GL_STREAM_DRAW);

	for (uint i = 0; i < MAX_PARTICLES; ++i)
	{
		glBufferSubData(GL_ARRAY_BUFFER, i * (sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)), sizeof(point3), &_particleBox.at(i)->position);
		glBufferSubData(GL_ARRAY_BUFFER, i * (sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)), sizeof(point3), &_particleBox.at(i)->velocity);
		glBufferSubData(GL_ARRAY_BUFFER, i * (sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)), sizeof(point3), &_particleBox.at(i)->acceleration);
		glBufferSubData(GL_ARRAY_BUFFER, i * (sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)), sizeof(color4), &_particleBox.at(i)->color);
		glBufferSubData(GL_ARRAY_BUFFER, i * (sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)), sizeof(float), &_particleBox.at(i)->life);
		glBufferSubData(GL_ARRAY_BUFFER, i * (sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)), sizeof(float), &_particleBox.at(i)->size);
	}

	// 1rst attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),                  // stride
		BUFFER_OFFSET(0)            // array buffer offset
		);

	// 2nd attribute buffer : velocity
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),                  // stride
		            // array buffer offset
		BUFFER_OFFSET(sizeof(point3)));

	// 3nd attribute buffer : accleration
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),                  // stride
		// array buffer offset
		BUFFER_OFFSET(sizeof(point3) * 2));

	// 4nd attribute buffer : color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),                  // stride
		// array buffer offset
		BUFFER_OFFSET(sizeof(point3) * 3));

	// 4nd attribute buffer : life
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		1,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),                  // stride
		// array buffer offset
		BUFFER_OFFSET(sizeof(point3) * 3 + sizeof(point4)));

	// 4nd attribute buffer : size
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(
		5,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		1,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		(sizeof(point3) * 3 + sizeof(point4) + 2 * sizeof(float)),                  // stride
		// array buffer offset
		BUFFER_OFFSET(sizeof(point3) * 3 + sizeof(point4) + sizeof(float)));

}