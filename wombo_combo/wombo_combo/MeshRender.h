#pragma once
#include "MovableActor.h"
#include "IRenderable.h"
#include "Mesh.h"
#include "Plane.h"


//In fact, I shall call it mesh renderable
class MeshRender
{
public:
	MeshRender();
	~MeshRender();
	void Init(Mesh* mesh, GLenum drawMode);
	void Render(Camera* cam, Shader* shader, color4& color, matrix4& model, float lineWidth = 1.0f, bool depth = true);
private:
	GLuint _vao, _vbo, _ebo;
	Mesh* _mesh;
	GLenum _drawMode;
};

