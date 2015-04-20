#pragma once
#include "Global.h"

class Particle
{
public:
	Particle();
	~Particle();
	bool Update(float dtime);

	point3 position;
	point3 velocity;
	point3 acceleration;
	point4 color;
	float size;
	float life;
	float TTL;
};

