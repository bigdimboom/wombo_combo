#pragma once

/*
*Reference :
*http://www.kfish.org/boids/pseudocode.html
*/

#include "Global.h"
#include "Mesh.h"
#include "Actor.h"
#include "IMovable.h"

class MovableActor : public Actor, public IMovable
{
public:
	MovableActor(point3 startPoint = point3(0.0f, 0.0f, 0.0f), point3 velocity = point3(0.0f, 0.0f, 0.0f));
	~MovableActor();
	inline float GetRadius() const { return this->radius; }
	inline Mesh* GetMesh() const { return this->mesh; }
	void SetVelocity(point3 velocity) { this->velocity = velocity; }
	void SetPosition(point3 position) { Actor::position = position; }
	void ApplyVelocity(point3 velocity) { this->velocity += velocity; }
	void SetRadius(float size);
	void SetMesh(Mesh* mesh);
	void Move(float dTime) override;
	inline const point3& GetVelocity() const { return this->velocity; }
protected:
	float radius;
	Mesh* mesh;
	point3 velocity;
};

