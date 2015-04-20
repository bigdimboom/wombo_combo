#pragma once
#include "Global.h"
#include "Particle.h"
#include "ParticleFactory.h"
#include <vector>
#include "Shader.h"
#include "Camera.h"



#define MAX_PARTICLES 100000

class ParticleEffect
{
public:
	ParticleEffect();
	~ParticleEffect();
	void Init();
	void Update(float dTime);
	void Render(Shader* shader, Camera* cam);
private:
	ParticleFactory _particleFactory;
	std::vector<Particle*> _particleBox;
private:
	void _InitParticlesData();
	void _InitDrawData();

	GLuint _vbo;
};

