#pragma once
#include "Global.h"
#include "Particle.h"
#include "ParticleFactory.h"
#include <vector>
#include "Shader.h"
#include "Camera.h"



#define MAX_PARTICLES 10000
//#define STATIC_BUFFER_DATA 0
//#define DYNAMIC_BUFFER_DATA 1


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

	GLuint _vao;
};

