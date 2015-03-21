#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Global.h"

class IRenderable
{
public:
	virtual ~IRenderable(){}
	virtual void Render(Camera* cam, point3* lightPos, Shader* shader) = 0;
};