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
		}
	}
}

void ParticleEffect::Render(Shader* shader, Camera* cam)
{
	assert(shader && cam);


}


void ParticleEffect::_InitParticlesData()
{
	_particleFactory.SetProperties(point3(0.0f, 10.0f, 0.0f),
		point3(-20.0f, -20.0f, -20.0f), point3(20.0f, 20.0f, 20.0f),
		point3(0.0f, -9.8f, 0.0f), color4(0.9, 0.2, 0.6, 1.0),
		0.8f, 1.0f,
		1.0f);

	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		Particle* p = _particleFactory.CreateSingleParticle(
			color4(glm::linearRand(0.5f, 1.0f),
			glm::linearRand(0.1f, 0.7f),
			glm::linearRand(0.5f, 0.7f), 1.0f), 
			/////
			glm::linearRand(1.5f, 2.0f));
		_particleBox.push_back(p);
	}
}



//point3 position; 0
//point3 velocity; 1
//point3 acceleration; 2
//point4 color; 3
//float life; 4
//float size; 5


void ParticleEffect::_InitDrawData()
{
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES *
		sizeof(point3) * 3 + sizeof(point4) + sizeof(float) * 2,
		NULL, GL_STREAM_DRAW);
}