#pragma once
#include "Boid.h"
#include <vector>
#include <map>
#include <string>
#include <glm/gtc/random.hpp>
#include <memory>

typedef std::shared_ptr<Mesh> Mesh_SPtr;


class Flock
{
public:
	Flock();
	virtual ~Flock();
	void AddBoid(Boid* boid); //initialise_positions
	virtual void MoveAll(float elaspedTime); //move_all_boids_to_new_positions
	point3 RuleCohesion(Boid* b);
	point3 RuleSeparation(Boid* b);
	point3 RuleAlignment(Boid* b);
	point3 RuleSpeedLimit(Boid* b);
protected:
	std::vector<Boid*> flock;
	std::map<std::string, Mesh_SPtr> meshes;
};

