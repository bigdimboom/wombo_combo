#pragma once
#include "Global.h"

typedef uint ActorId;

class Actor
{
public:
	Actor();
	~Actor();
	const point4& GetPosition(void) const { return _location; }
	const ActorId GetId(void) const { return _id; }
protected:
	point4 _location;
private:
	ActorId _id;
};

