#pragma once
#include "Camera.h"

class FreeCamera :
	public Camera
{
public:
	FreeCamera(point3 cameraPos = point3(0.0f, 0.0f, 3.0f),
		point3 worldUp = point3(0.0f, 1.0f, 0.0f));
	~FreeCamera();
	void Rotate(Camera_Rotate dir, GLfloat angle) override;
};

