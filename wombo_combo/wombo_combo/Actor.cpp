#include "Actor.h"


Actor::Actor()
	:position(0.0f)
{
	UniqueID a;
	_id = a.id;
}

Actor::Actor(point3 point)
	:position(point)
{
	UniqueID a;
	_id = a.id;
}

Actor::~Actor()
{
}