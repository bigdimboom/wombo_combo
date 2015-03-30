#include "ActorMovable.h"


ActorMovable::ActorMovable(point3 startPoint, point3 velocity)
	: Actor(startPoint),
	_mesh(nullptr),
	_velocity(velocity)
{
}

ActorMovable::~ActorMovable()
{
}

void ActorMovable::SetRadius(float radius)
{
	_radius = radius;
}

void ActorMovable::SetMesh(Mesh* mesh)
{
	assert(mesh != nullptr);
	_mesh = mesh;
}

void ActorMovable::Move(float dTime)
{
	//for debugging purpose
	if (_velocity == point3(0.0f, 0.0f, 0.0f))
	{
		std::cout << "Velocity is " << glm::to_string(_velocity) << std::endl;
	}

	position += dTime *_velocity;
}