#include "Particle.h"


Particle::Particle()
{
}


Particle::~Particle()
{
}

bool Particle::Update(float dtime)
{
	TTL -= dtime;
	life += dtime;

	if (TTL > 0.0f)
	{
		return true;
	}

	return false;
}