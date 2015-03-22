#pragma once
#include "Global.h"
#include "UniqueID.h"

typedef uint ActorId;

class Actor
{
public:
	Actor();
	Actor(point3 position);
	~Actor();
	const point3& GetPosition(void) const { return position; }
	const ActorId GetId(void) const { return _id; }
protected:
	point3 position; // the center location of a object
private:
	ActorId _id;
};

