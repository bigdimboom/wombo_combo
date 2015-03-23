#pragma once

/*
*Reference :
*http://www.kfish.org/boids/pseudocode.html
*/

#include "Global.h"
#include "Mesh.h"
#include "Actor.h"

class Boid : public Actor
{
public:
	Boid(point3 startPoint = point3(0.0f, 0.0f, 0.0f), point3 velocity = point3(0.0f, 0.0f, 0.0f));
	~Boid();
	inline float GetRadius() const { return _radius; }
	inline Mesh* GetMesh() const { return _mesh; }
	void SetVelocity(point3 velocity) { _velocity = velocity; }
	void ApplyVelocity(point3 velocity) { _velocity += velocity; }
	void SetRadius(float size);
	void SetMesh(Mesh* mesh);
	void Move(float elapsedTime);
	inline const point3& GetVelocity() const { return _velocity; }
private:
	float _radius;
	Mesh* _mesh;
	point3 _velocity;
};

