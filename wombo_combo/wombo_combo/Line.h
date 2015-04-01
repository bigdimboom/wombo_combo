#pragma once
#include "Mesh.h"

class Line : public Mesh
{
public:
	Line(const point4& from, const point4& to);
	~Line();
	void Init(const point4& from, const point4& to);
private:
	bool _isBuilt;
};

