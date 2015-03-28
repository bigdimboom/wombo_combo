#pragma once
#include "Flock.h"
#include "IRenderable.h"
#include "Cube.h"

class FlockRenderable :
	public Flock, IRenderable
{
public:
	FlockRenderable();
	~FlockRenderable();
	void Init();
	void Render(Camera* cam, point3* lightPos, Shader* shader) override;
private:
	GLuint _vao, _vbo, _ebo;
	bool _isInited;
};

