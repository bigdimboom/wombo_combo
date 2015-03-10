#pragma once
#include "Camera.h"
#include "Global.h"

/*
The frustrum class is ported from
http://www.rastertek.com/dx10tut16.html
*/

class Frustum
{
public:
	Frustum();
	~Frustum();

	void Build(Camera* cam, float screenDepth);
	bool IsCubeInFrustum(const point3 &center, float cubeRadius);

public:
	enum {
		FRUSTUM_TOP = 0,
		FRUSTUM_BOTTOM,
		FRUSTUM_LEFT,
		FRUSTUM_RIGHT,
		FRUSTUM_NEAR,
		FRUSTUM_FAR,
	};

	static enum {
		FRUSTUM_OUTSIDE = 0,
		FRUSTUM_INTERSECT,
		FRUSTUM_INSIDE,
	};

private:

	point4 _plane[6];
	matrix4 _mat;
};

