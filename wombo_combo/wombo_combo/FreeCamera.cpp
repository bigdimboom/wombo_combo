#include "FreeCamera.h"


FreeCamera::FreeCamera(point3 pos, point3 worldUp)
	:Camera(pos, worldUp)
{
}


FreeCamera::~FreeCamera()
{
}

void FreeCamera::Rotate(Camera_Rotate dir, GLfloat angle)
{
	Camera::Rotate(dir, angle);
	if (_pitch > 80.0f)
	{
		_pitch = 80.0f;
	}
	if (_pitch < -80.0f)
	{
		_pitch = -80.0f;
	}
	Update();
}