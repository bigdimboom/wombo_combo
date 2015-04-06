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
	std::cout << "Error" << __LINE__ << std::endl;
	return point3(0.0f, 0.0f, 0.0f);
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
		if (!planes[i].IsInside(point))
			return false;
	}
	return true;
}

bool Frustum::IsSphereInside(const point3& point, const float radius)
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (!planes[i].IsInside(point, radius))
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
	float nearHeight_half = dNear * halfTanFOV;
	float farHeight_half = dFar * halfTanFOV;
	float nearWidth_half = nearHeight_half * aspect;
	float farWidth_half = farHeight_half * aspect;

	point3  nearCenter = cam->GetPosition() + dNear * cam->GetFront();
	point3  farCenter = cam->GetPosition() + dFar * cam->GetFront();

	nearClip[UP_LEFT] = nearCenter + (camera->GetUp() * nearHeight_half) -
		(camera->GetRight() * nearWidth_half);
	nearClip[UP_RIGHT] = nearCenter + (camera->GetUp() * nearHeight_half) +
		(camera->GetRight() * nearWidth_half);
	nearClip[BOTTOM_RIGHT] = nearCenter - (camera->GetUp() * nearHeight_half) + (camera->GetRight() * nearWidth_half);
	nearClip[BOTTOM_LEFT] = nearCenter - (camera->GetUp() * nearHeight_half) - (camera->GetRight() * nearWidth_half);

	farClip[UP_LEFT] = farCenter + (camera->GetUp() * farHeight_half) -
		(camera->GetRight() * farWidth_half);
	farClip[UP_RIGHT] = farCenter + (camera->GetUp() * farHeight_half) +
		(camera->GetRight() * farWidth_half);
	farClip[BOTTOM_RIGHT] = farCenter - (camera->GetUp() * farHeight_half) + (camera->GetRight() * farWidth_half);
	farClip[BOTTOM_LEFT] = nearCenter - (camera->GetUp() * farHeight_half) - (camera->GetRight() * farWidth_half);

	planes[NEARD].Set(nearClip[UP_LEFT], nearClip[UP_RIGHT], nearClip[BOTTOM_RIGHT]);
	planes[FARD].Set(nearClip[UP_RIGHT], nearClip[UP_LEFT], nearClip[BOTTOM_LEFT]);

	planes[LEFT].Set(nearClip[UP_LEFT], nearClip[BOTTOM_LEFT], farClip[BOTTOM_LEFT]);
	planes[RIGHT].Set(nearClip[BOTTOM_RIGHT], nearClip[UP_RIGHT], farClip[BOTTOM_RIGHT]);

	planes[TOP].Set(nearClip[UP_RIGHT], nearClip[UP_LEFT], farClip[UP_LEFT]);
	planes[BOTTOM].Set(nearClip[BOTTOM_LEFT], nearClip[BOTTOM_RIGHT], farClip[BOTTOM_RIGHT]);
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
			return false;
		// if some corners are out and others are in
		else if (out)
			result = INTERSECT;
	}
	return true;
}


void Frustum::Render()
{

}