#include "MovableActor.h"


MovableActor::MovableActor(point3 startPoint, point3 velocity)
	: Actor(startPoint),
	_mesh(nullptr),
	_velocity(velocity)
{
}

MovableActor::~MovableActor()
{
}

void MovableActor::SetRadius(float radius)
{
	_radius = radius;
}

void MovableActor::SetMesh(Mesh* mesh)
{
	assert(mesh != nullptr);
	_mesh = mesh;
}

void MovableActor::Move(float dTime)
{
	//for debugging purpose
	if (_velocity == point3(0.0f, 0.0f, 0.0f))
	{
		std::cout << "Velocity is " << glm::to_string(_velocity) << std::endl;
	}

	position += dTime *_velocity;
}