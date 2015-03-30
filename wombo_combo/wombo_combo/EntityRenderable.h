#pragma once
#include "ActorMovable.h"
#include "IRenderable.h"
#include "Mesh.h"
#include "Plane.h"


class EntityRenderable :
	public ActorMovable, IRenderable
{
public:
	EntityRenderable();
	~EntityRenderable();
	void Init(Mesh* mesh);
	void Render(Camera* cam, point3* lightPos, Shader* shader) override;
	void Scale(float scale);
	void SetPosition(point3 pos) { position = pos; }
	bool IsCollideWith_NaiveVersion(point4& a, point4& b, point4& c);
private:
	GLuint _vao, _vbo, _ebo;
	matrix4 _scaleModel;
};

