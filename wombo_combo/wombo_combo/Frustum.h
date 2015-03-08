#pragma once
#include "Camera.h"

class Frustum
{
public:
	Frustum();
	~Frustum();

//	void SetFrustum(float angle, float ratio, float nearD, float farD);
//	void SetCamera(Camera* cam);
//	bool CubeInFrustum(const point4 center, float cubeRadius);
//
//public:
//	enum {
//		FRUSTUM_TOP = 0,
//		FRUSTUM_BOTTOM,
//		FRUSTUM_LEFT,
//		FRUSTUM_RIGHT,
//		FRUSTUM_NEAR,
//		FRUSTUM_FAR,
//	};
//
//	static enum {
//		FRUSTUM_OUTSIDE = 0,
//		FRUSTUM_INTERSECT,
//		FRUSTUM_INSIDE,
//	};
//
//	//Plane3D planes[6];
//
//	point3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
//	point3 farTopLeft, farTopRight, farBottomLeft, farBottomRight;
//
//	float nearDistance, farDistance;
//	float nearWidth, nearHeight;
//	float farWidth, farHeight;
//	float ratio, angle, tang;
};

