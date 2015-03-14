#include "Frustum.h"

static point3 GetPoint(point3 center, float radius, int i)
{
	switch (i)
	{
	case 0:
		return point3(center.x - radius, center.y + radius, center.z - radius);
	case 1:
		return point3(center.x + radius, center.y + radius, center.z - radius);
	case 2:
		return point3(center.x - radius, center.y - radius, center.z - radius);
	case 3:
		return point3(center.x + radius, center.y - radius, center.z - radius);
	case 4:
		return point3(center.x - radius, center.y + radius, center.z + radius);
	case 5:
		return point3(center.x + radius, center.y + radius, center.z + radius);
	case 6:
		return point3(center.x - radius, center.y - radius, center.z + radius);
	case 7:
		return point3(center.x + radius, center.y - radius, center.z + radius);
	}
	//return point3(0.0f, 0.0f, 0.0f);
}



Frustum::Frustum()
{
	this->dNear = 1.0f;
	this->dFar = 1000.f;
	this->aspect = 1.0f;
	this->fov = 60.f;
	this->camera = nullptr;
}


Frustum::~Frustum()
{
}

bool Frustum::IsPointInside(const point3& point)
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (!planes[i].Inside(point))
			return false;
	}
	return true;
}

bool Frustum::IsSphereInside(const point3& point, const float radius)
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (!planes[i].Inside(point, radius))
			return false;
	}
	// otherwise we are fully in view
	return true;
}

const Plane& Frustum::Get(Side side)
{
	return this->planes[side];
}

void Frustum::SetFOV(float fov)
{
	this->fov = fov;
	Init(this->camera, this->fov,
		this->aspect, this->dNear, this->dFar);
}

void Frustum::SetAspect(float aspect)
{
	this->aspect = aspect;
	Init(this->camera, this->fov,
		this->aspect, this->dNear, this->dFar);
}

void Frustum::SetNear(float nearClip)
{
	this->dNear = nearClip;
	Init(this->camera, this->fov,
		this->aspect, this->dNear, this->dFar);
}

void Frustum::SetFar(float farClip)
{
	this->dFar = farClip;
	Init(this->camera, this->fov,
		this->aspect, this->dNear, this->dFar);
}

void Frustum::SetCamera(Camera* camera)
{
	this->camera = camera;
	Init(this->camera, this->fov,
		this->aspect, this->dNear, this->dFar);
}

void Frustum::Init(Camera* cam, const float fov,
	const float aspect, const float nearD, const float farD)
{
	this->camera = cam;
	this->fov = fov;
	this->aspect = aspect;
	this->dNear = nearD;
	this->dFar = farD;

	float halfTanFOV = glm::tan(glm::radians(this->fov / 2.0f));

	point3 nearRight = this->camera->GetPosition() + (this->dNear * halfTanFOV) * this->aspect
		* this->camera->GetRight();

	point3 farRight = this->camera->GetPosition() +(this->dFar * halfTanFOV) * this->aspect
		* this->camera->GetRight();

	point3 nearUp = this->camera->GetPosition() + (this->dNear * halfTanFOV) * this->camera->GetUp();

	point3 farUp = this->camera->GetPosition() + (this->dFar * halfTanFOV) * this->camera->GetUp();

	// points start in the upper right and go around clockwise
	nearClip[0] = (this->camera->GetPosition() + this->dNear * cam->GetFront()) - nearRight + nearUp; // leftup
	nearClip[1] = (this->camera->GetPosition() + this->dNear * cam->GetFront()) + nearRight + nearUp; // rtup
	nearClip[2] = (this->camera->GetPosition() + this->dNear * cam->GetFront()) + nearRight - nearUp; // rtdown
	nearClip[3] = (this->camera->GetPosition() + this->dNear * cam->GetFront()) - nearRight - nearUp; // leftdown

	farClip[0] = (this->camera->GetPosition() + this->dFar * cam->GetFront()) - farRight + farUp;
	farClip[1] = (this->camera->GetPosition() + this->dFar * cam->GetFront()) + farRight + farUp;
	farClip[2] = (this->camera->GetPosition() + this->dFar * cam->GetFront()) + farRight - farUp;
	farClip[3] = (this->camera->GetPosition() + this->dFar * cam->GetFront()) - farRight - farUp;

	planes[NEARD].Set(nearClip[0], nearClip[1], nearClip[2]);
	planes[FARD].Set(farClip[0], farClip[3], nearClip[2]);

	planes[LEFT].Set(farClip[0], nearClip[0], nearClip[3]);
	planes[RIGHT].Set(nearClip[1], farClip[1], farClip[2]);

	planes[TOP].Set(nearClip[0], farClip[0], farClip[1]);
	planes[BOTTOM].Set(nearClip[1], farClip[1], farClip[3]);
}

bool Frustum::IsCubeInside(const point3& center, const float cubeRadius)
{
	int result = INSIDE, out, in;

	// for each plane do ...
	for (int i = 0; i < 6; i++) {

		// reset counters for corners in and out
		out = 0; in = 0;
		// for each corner of the box do ...
		// get out of the cycle as soon as a box as corners
		// both inside and out of the frustum
		for (int k = 0; k < 8 && (in == 0 || out == 0); k++) {

			// is the corner outside or inside
			if (planes[i].PlaneDotCoord(GetPoint(center, cubeRadius, k)) < 0)
				out++;
			else
				in++;
		}
		//if all corners are out
		if (!in)
			return (OUTSIDE);
		// if some corners are out and others are in
		else if (out)
			result = INTERSECT;
	}
	return(result);
}


void Frustum::Render()
{

}