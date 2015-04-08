#include "Frustum.h"

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

const Plane& Frustum::Get(Side side)
{
	return this->planes[side];
}

void Frustum::Set(Camera* cam)
{
	assert(cam != nullptr);
	this->camera = cam;
	this->fov = cam->GetFov();
	this->aspect = cam->GetDisplayRatio();
	this->dNear = cam->GetNearDistance();
	this->dFar = cam->GetFarDistance();

	float halfTanFOV = glm::tan(glm::radians(this->fov / 2.0f));
	float nearHeight_half = dNear * halfTanFOV;
	float farHeight_half = dFar * halfTanFOV;
	float nearWidth_half = nearHeight_half * aspect;
	float farWidth_half = farHeight_half * aspect;

	point3  nearCenter = cam->GetPosition() + dNear * cam->GetFront();
	point3  farCenter = cam->GetPosition() +  dFar * cam->GetFront();

	nearClip[UP_LEFT] = nearCenter + (camera->GetUp() * nearHeight_half) -
		(camera->GetRight() * nearWidth_half);
	nearClip[UP_RIGHT] = nearCenter + (camera->GetUp() * nearHeight_half) +
		(camera->GetRight() * nearWidth_half);

	nearClip[BOTTOM_RIGHT] = nearCenter - (camera->GetUp() * nearHeight_half) + (camera->GetRight() * nearWidth_half);
	nearClip[BOTTOM_LEFT] = nearCenter - (camera->GetUp() * nearHeight_half) -(camera->GetRight() * nearWidth_half);

	farClip[UP_LEFT] = farCenter + (camera->GetUp() * farHeight_half) -
		(camera->GetRight() * farWidth_half);
	farClip[UP_RIGHT] = farCenter + (camera->GetUp() * farHeight_half) +
		(camera->GetRight() * farWidth_half);

	farClip[BOTTOM_RIGHT] = farCenter - (camera->GetUp() * farHeight_half) + (camera->GetRight() * farWidth_half);
	farClip[BOTTOM_LEFT] = farCenter - (camera->GetUp() * farHeight_half) - (camera->GetRight() * farWidth_half);
	//////////////////////////////////////////////////////////////////////////////////////////////
	planes[NEARD].Set(nearClip[UP_LEFT], nearClip[UP_RIGHT], nearClip[BOTTOM_RIGHT]);
	planes[FARD].Set(nearClip[UP_RIGHT], nearClip[UP_LEFT], nearClip[BOTTOM_LEFT]);

	planes[LEFT].Set(nearClip[UP_LEFT], nearClip[BOTTOM_LEFT], farClip[BOTTOM_LEFT]);
	planes[RIGHT].Set(nearClip[BOTTOM_RIGHT], nearClip[UP_RIGHT], farClip[BOTTOM_RIGHT]);

	planes[TOP].Set(nearClip[UP_RIGHT], nearClip[UP_LEFT], farClip[UP_LEFT]);
	planes[BOTTOM].Set(nearClip[BOTTOM_LEFT], nearClip[BOTTOM_RIGHT], farClip[BOTTOM_RIGHT]);

	//DebugDrawManager::getInstance().
	//	AddFrustum(point4(nearClip[UP_LEFT],1.0f), point4(nearClip[UP_RIGHT],1.0f),
	//	point4(nearClip[BOTTOM_RIGHT], 1.0f), point4(nearClip[BOTTOM_LEFT], 1.0f), 
	//	point4(farClip[UP_LEFT], 1.0f), point4(farClip[UP_RIGHT], 1.0f),
	//	point4(farClip[BOTTOM_RIGHT], 1.0f), point4(farClip[BOTTOM_LEFT], 1.0f),color4(0.1f, 0.4f, 0.5f, 1.0f), 100.0f, 1.5f, false);
}

bool Frustum::IsCubeInside(const point3& center, const float cubeRadius) const
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (planes[i].IsCubeInstersect(center, cubeRadius))
		{
			return true;
		}
	}
	return false;
}

