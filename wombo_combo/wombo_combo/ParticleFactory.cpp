#include "ParticleFactory.h"


ParticleFactory::ParticleFactory()
	:_isPropertiesSet(false)
{
}


ParticleFactory::~ParticleFactory()
{
}


void ParticleFactory::SetProperties(point3 initPosition, point3 initVelocityMin, point3 initVelocityMax, point3 acceleration,
	color4 color, float lifeMin, float lifeMax, float size)
{
	_initPosition = initPosition;
	_velocityMin = initVelocityMin;
	_velocityMax = initVelocityMax;
	_acceleration = acceleration;
	_color = color;
	_lifeMin = lifeMin;
	_lifeMax = lifeMax;
	_size = size;
	_isPropertiesSet = true;
}


Particle* ParticleFactory::CreateSingleParticle()
{
	assert(_isPropertiesSet);
	Particle* p = new Particle;

	p->position = _initPosition;
	p->acceleration = _acceleration;
	p->velocity = point3(
		glm::linearRand(_velocityMin.x, _velocityMax.x),
		glm::linearRand(_velocityMin.y, _velocityMax.y),
		glm::linearRand(_velocityMin.z, _velocityMax.z));

	p->life = 0.0f;
	p->TTL = glm::linearRand(_lifeMin, _lifeMax);

	p->color = _color;
	p->size = _size;

	return p;
}

Particle* ParticleFactory::CreateSingleParticle(color4 color, float size)
{
	assert(_isPropertiesSet);
	_color = color;
	_size = size;
	return CreateSingleParticle();
}