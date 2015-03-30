#pragma once

/*
*Reference :
*http://www.kfish.org/boids/pseudocode.html
*/

#include "Global.h"
#include "Mesh.h"
#include "Actor.h"
#include "IMovable.h"

class ActorMovable : public Actor, public IMovable
{
public:
	ActorMovable(point3 startPoint = point3(0.0f, 0.0f, 0.0f), point3 velocity = point3(0.0f, 0.0f, 0.0f));
	~ActorMovable();
	inline float GetRadius() const { return _radius; }
	inline Mesh* GetMesh() const { return _mesh; }
	void SetVelocity(point3 velocity) { _velocity = velocity; }
	void ApplyVelocity(point3 velocity) { _velocity += velocity; }
	void SetRadius(float size);
	void SetMesh(Mesh* mesh);
	void Move(float dTime) override;
	inline const point3& GetVelocity() const { return _velocity; }
private:
	float _radius;
	Mesh* _mesh;
	point3 _velocity;
};

