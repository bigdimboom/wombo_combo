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
		v1 = RuleSeparation(flock[i]);
		v2 = RuleCohesion(flock[i]);
		v3 = RuleAlignment(flock[i]);

		flock[i]->ApplyVelocity(v1 + v2 + v3);

		v4 = RuleSpeedLimit(flock[i]);

		flock[i]->SetVelocity(v4);
		flock[i]->Move(elaspedTime);
	}
}

point3 Flock::RuleSeparation(Boid* b)
{
	point3 pVelocity; //perceived velocity
	for (uint i = 0; i < flock.size(); ++i)
	{
		if (flock[i] != b)
		{
			if (glm::distance(b->GetPosition(), flock[i]->GetPosition()) < b->GetRadius() * 100)
			{
				pVelocity += flock[i]->GetVelocity() - b->GetVelocity();
			}
		}
	}

	pVelocity.x *= -1;
	pVelocity.z *= -1;
	//pVelocity.y = 0.0f;

	return pVelocity;
}

point3 Flock::RuleCohesion(Boid* b)
{
	point3 pCenter; //perceived center
	int count = 0;
	for (uint i = 0; i < flock.size(); ++i)
	{
		if (flock[i] != b)
		{
			if (glm::distance(b->GetPosition(), flock[i]->GetPosition()) < b->GetRadius() * 10)
			{
				++count;
			}
			pCenter += flock[i]->GetPosition();
		}
	}

	pCenter /= flock.size() - 1;
	if (count == 0)
	{
		pCenter = (pCenter - b->GetPosition()) /= 100;
	}
	else
	{
		pCenter = (pCenter - b->GetPosition()) /= count;
	}

	pCenter = glm::normalize(pCenter);
	//pCenter.y = 0.0f;

	return pCenter;
}

point3 Flock::RuleAlignment(Boid* b)
{
	point3 pVelocity; //perceived velocity
	int count = 0;
	for (uint i = 0; i < flock.size(); ++i)
	{
		if (flock[i] != b)
		{
			if (glm::distance(b->GetPosition(), flock[i]->GetPosition()) < b->GetRadius() * 100)
			{
				pVelocity += flock[i]->GetVelocity();
				++count;
			}
		}
	}

	pVelocity /= flock.size() - 1;
	if (count == 0)
	{
		pVelocity = (pVelocity - b->GetVelocity()) /= 8;
	}
	else
	{
		pVelocity = (pVelocity - b->GetVelocity()) /= count;
	}
	pVelocity = glm::normalize(pVelocity);
	//pVelocity.y = 0.0f;

	return pVelocity;
}

point3 Flock::RuleSpeedLimit(Boid* b)
{
	float speedLimit = 1.0f;
	point3 v = b->GetVelocity();

	if (glm::length(b->GetVelocity()) > speedLimit)
	{
		v = glm::normalize(b->GetVelocity()) * speedLimit;
	}

	return v;
}