#include "MovableActor.h"


MovableActor::MovableActor(point3 startPoint, point3 velocity)
	: Actor(startPoint),
	mesh(nullptr),
	velocity(velocity)
{
}

MovableActor::~MovableActor()
{
}

void MovableActor::SetRadius(float radius)
{
	this->radius = radius;
}

void MovableActor::SetMesh(Mesh* mesh)
{
	assert(mesh != nullptr);
	this->mesh = mesh;
}

void MovableActor::Move(float dTime)
{
	//for debugging purpose
	if (this->velocity == point3(0.0f, 0.0f, 0.0f))
	{
		std::cout << "Velocity is " << glm::to_string(this->velocity) << std::endl;
	}

	position += dTime *this->velocity;
}