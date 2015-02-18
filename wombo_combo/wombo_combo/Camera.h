#pragma once
#include "Global.h"

class Camera
{
public:
	Camera();
	~Camera();
private:
	matrix4 _viewMat;
};

