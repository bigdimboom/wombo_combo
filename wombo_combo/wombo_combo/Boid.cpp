#include "Boid.h"


Boid::Boid(point3 startPoint, point3 velocity)
	: Actor(startPoint),
	_mesh(nullptr),
	_velocity(velocity)
{
}

Boid::~Boid()
{
}

void Boid::SetRadius(float radius)
{
	_radius = radius;
}

void Boid::SetMesh(Mesh* mesh)
{
	assert(mesh != nullptr);
	_mesh = mesh;
}

void Boid::Move(float elapsedTime)
{
	//for debugging purpose
	if (_velocity == point3(0.0f, 0.0f, 0.0f))
	{
		std::cout << "Velocity is " << glm::to_string(_velocity) << std::endl;
	}

	position += elapsedTime *_velocity;
}