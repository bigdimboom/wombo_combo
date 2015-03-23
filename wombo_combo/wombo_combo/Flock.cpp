#include "Flock.h"


Flock::Flock()
{
}


Flock::~Flock()
{
	for (uint i = 0; i < flock.size(); ++i)
	{
		delete flock[i];
	}
	meshes.clear();
}

void Flock::AddBoid(Boid* boid)
{
	assert(boid != nullptr);
	flock.push_back(boid);
}

void Flock::MoveAll(float elaspedTime)
{
	point3 v1, v2, v3, v4;
	for (uint i = 0; i < flock.size(); ++i)
	{
		v1 = RuleCohesion(flock[i]);
		v2 = RuleSeparation(flock[i]);
		v3 = RuleAlignment(flock[i]);

		flock[i]->ApplyVelocity(v2);

		v4 = RuleSpeedLimit(flock[i]);
		
		flock[i]->SetVelocity(v4);
		flock[i]->Move(elaspedTime);
	}
}

point3 Flock::RuleSeparation(Boid* b)
{
	point3 pVelocity; //perceived velocity

	return pVelocity;
}

point3 Flock::RuleCohesion(Boid* b)
{
	point3 pCenter; //perceived center
	for (uint i = 0; i < flock.size(); ++i)
	{
		if (flock[i] != b)
		{
			pCenter = pCenter + flock[i]->GetPosition();
		}
	}

	pCenter /= flock.size() - 1;
	pCenter = (pCenter - b->GetPosition()) /= 500;
	
	pCenter = glm::normalize(pCenter);

	pCenter.y = 0.0f;

	return pCenter;
}

point3 Flock::RuleAlignment(Boid* b)
{
	point3 pVelocity; //perceived velocity

	return pVelocity;
}

point3 Flock::RuleSpeedLimit(Boid* b)
{
	float speedLimit = 2.0f;
	point3 v = b->GetVelocity();

	if (glm::length(b->GetVelocity()) > speedLimit)
	{
		v = glm::normalize(b->GetVelocity()) * speedLimit;
	}

	return v;
}