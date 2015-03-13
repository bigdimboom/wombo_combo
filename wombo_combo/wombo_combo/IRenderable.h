#pragma once

#include "Camera.h"
#include "Shader.h"

class IRenderable
{
public:
	virtual ~IRenderable(){}
	virtual void Render(Camera &camera, Shader& shader) = 0;
	virtual void InitRenderData() = 0;
};