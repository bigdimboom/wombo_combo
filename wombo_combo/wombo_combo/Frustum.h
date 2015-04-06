#pragma once
#include "Camera.h"
#include "Global.h"
#include "Plane.h"

/*
The frustrum class is ported from
http://www.rastertek.com/dx10tut16.html
*/

class Frustum
{

public:
	enum Side
	{
		NEARD = 0,
		FARD,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NumPlanes
	};

	enum
	{
		UP_LEFT = 0,
		UP_RIGHT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT,
		NUM_CONNERS
	};
	enum { OUTSIDE, INTERSECT, INSIDE };


	Plane planes[NumPlanes]; // planes of the frusum in camera space
	point3 nearClip[NUM_CONNERS]; // verts of the near clip plane in camera space
	point3 farClip[NUM_CONNERS]; // verts of the far clip plane in camera space
	float fov; // field of view in degree
	float aspect; // aspect ratio - width divided by height
	float dNear; // near clipping distance
	float dFar; // far clipping distanc
	Camera* camera;

public:
	Frustum();
	~Frustum();
	bool IsPointInside(const point3& point);
	bool IsSphereInside(const point3& point, const float radius);
	bool IsCubeInside(const point3& center, const float cubeRadius);

	const Plane &Get(Side side);
	void SetCamera(Camera* camera);
	void SetFOV(float fov);
	void SetAspect(float aspect);
	void SetNear(float nearClip);
	void SetFar(float farClip);
	void Init(Camera* cam, const float fov, const float aspect,
		const float nearD, const float farD);
	void Render();

};

