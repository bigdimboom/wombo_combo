#include "Frustum.h"

float PlaneDotVector(point4 &p, point3 v)
{
	return p.x * v.x + p.y * v.y + p.z * v.z + p.w;
}


Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::Build(Camera* cam, float screenDepth)
{
	float zDepth, r;
	matrix4 view = *(cam->GetViewMatrix());
	matrix4 proj = *(cam->GetProjMatrix());
	zDepth = -proj[3][2] / proj[2][2];

	r = screenDepth / (screenDepth - zDepth);

	proj[2][2] = r;
	proj[3][2] = -r * zDepth;

	_mat = proj * view;

	_plane[FRUSTUM_NEAR].x = _mat[0][3] + _mat[0][2];
	_plane[FRUSTUM_NEAR].y = _mat[1][3] + _mat[1][2];
	_plane[FRUSTUM_NEAR].z = _mat[2][3] + _mat[2][2];
	_plane[FRUSTUM_NEAR].w = _mat[3][3] + _mat[3][2];

	_plane[FRUSTUM_FAR].x = _mat[0][3] - _mat[0][2];
	_plane[FRUSTUM_FAR].y = _mat[1][3] - _mat[1][2];
	_plane[FRUSTUM_FAR].z = _mat[2][3] - _mat[2][2];
	_plane[FRUSTUM_FAR].w = _mat[3][3] - _mat[3][2];

	_plane[FRUSTUM_LEFT].x = _mat[0][3] + _mat[0][0];
	_plane[FRUSTUM_LEFT].y = _mat[1][3] + _mat[1][0];
	_plane[FRUSTUM_LEFT].z = _mat[2][3] + _mat[2][0];
	_plane[FRUSTUM_LEFT].w = _mat[3][3] + _mat[3][0];

	_plane[FRUSTUM_RIGHT].x = _mat[0][3] - _mat[0][0];
	_plane[FRUSTUM_RIGHT].y = _mat[1][3] - _mat[1][0];
	_plane[FRUSTUM_RIGHT].z = _mat[2][3] - _mat[2][0];
	_plane[FRUSTUM_RIGHT].w = _mat[3][3] - _mat[3][0];

	_plane[FRUSTUM_TOP].x = _mat[0][3] - _mat[0][1];
	_plane[FRUSTUM_TOP].y = _mat[1][3] - _mat[1][1];
	_plane[FRUSTUM_TOP].z = _mat[2][3] - _mat[2][1];
	_plane[FRUSTUM_TOP].w = _mat[3][3] - _mat[3][1];

	_plane[FRUSTUM_BOTTOM].x = _mat[0][3] + _mat[0][1];
	_plane[FRUSTUM_BOTTOM].y = _mat[1][3] + _mat[1][1];
	_plane[FRUSTUM_BOTTOM].z = _mat[2][3] + _mat[2][1];
	_plane[FRUSTUM_BOTTOM].w = _mat[3][3] + _mat[3][1];

	//Normalize it
	for (int i = 0; i < 6; ++i)
	{
		_plane[i] = glm::normalize(_plane[i]);
	}
}


//test 8 points in a cube
bool Frustum::CubeInFrustum(const point3 center, float cubeRadius)
{
	for (int i = 0; i < 6; ++i)
	{
		if (PlaneDotVector(_plane[i], point3(center.x - cubeRadius, center.y + cubeRadius, center.z - cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x + cubeRadius, center.y + cubeRadius, center.z - cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x - cubeRadius, center.y - cubeRadius, center.z - cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x + cubeRadius, center.y - cubeRadius, center.z - cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x - cubeRadius, center.y + cubeRadius, center.z + cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x + cubeRadius, center.y + cubeRadius, center.z + cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x - cubeRadius, center.y - cubeRadius, center.z + cubeRadius)) >= 0.0f){ continue; }
		if (PlaneDotVector(_plane[i], point3(center.x + cubeRadius, center.y - cubeRadius, center.z + cubeRadius)) >= 0.0f){ continue; }
		return false;
	}
	return true;
}