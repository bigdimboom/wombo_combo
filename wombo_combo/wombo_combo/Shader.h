#pragma once
#include "Global.h"
#include <map>

class Shader
{
public:
	Shader();
	~Shader();
	bool Comiple(const char *vsPath, const char *fsPath);
	inline GLuint GetID() const { return _program; }
	inline void Use() { glUseProgram(_program); }
	void Delete();
private:
	GLuint _program;
private:
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {}
	GLuint CreateShader(GLenum eShaderType, const char *strShaderFile);
};