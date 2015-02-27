#pragma once
#include "Global.h"

typedef uint ActorId;

class Actor
{
public:
	Actor();
	~Actor();
	const point3& GetPosition(void) const { return _location; }
	const ActorId GetId(void) const { return _id; }
protected:
	point3 _location;
private:
	ActorId _id;
};

