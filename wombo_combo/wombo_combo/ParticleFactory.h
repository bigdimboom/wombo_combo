#pragma once
#include "Global.h"
#include "Particle.h"
#include <glm/gtc/random.hpp>

class ParticleFactory
{
public:
	ParticleFactory();
	~ParticleFactory();

	void SetProperties(point3 initPosition, point3 initVelocityMin, point3 initVelocityMax, point3 acceleration,
		 color4 color, float lifeMin, float lifeMax, float size);

	Particle* CreateSingleParticle();
	Particle* CreateSingleParticle(color4 color, float size);

private:
	bool _isPropertiesSet;
	point3 _initPosition;
	point3 _velocityMin;
	point3 _velocityMax;
	point3 _acceleration;
	color4 _color;
	float _lifeMax;
	float _lifeMin;
	float _size;
};

