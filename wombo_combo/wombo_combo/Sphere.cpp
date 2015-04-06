#include "Sphere.h"


Sphere::Sphere()
{
	_isBuilt = false;
	Build();
}


Sphere::~Sphere()
{
}

void Sphere::Build()
{
	assert(_isBuilt != true);
	_Tetrahedron();
	this->SetVertSize(_verts.size());
	memcpy(this->GetVerts(), &_verts[0], sizeof(point4) * _verts.size());
	SetIdxSize(_indices.size());
	memcpy(this->GetIndxs(), &_indices[0], sizeof(uint) * _indices.size());
	_edges.clear();
	_verts.clear();
	_indices.clear();
	_isBuilt = true;
}


void Sphere::_Tetrahedron() 
{
	point4 tera[4] = { point4(0.0f, 0.0f, 1.0f, 1.0f), point4(0.0f, 0.942809f,
		-0.333333f, 1.0f), point4(-0.816497f, -0.471405f, -0.333333f, 1.0f),
		point4(0.816497f, -0.471405f, -0.333333f, 1.0f) };

	_verts.push_back(tera[0]);
	_verts.push_back(tera[1]);
	_verts.push_back(tera[2]);
	_verts.push_back(tera[3]);

	_Subdivided(0, 1, 2, LOD);
	_Subdivided(3, 2, 1, LOD);
	_Subdivided(0, 3, 1, LOD);
	_Subdivided(0, 2, 3, LOD);

}

//i0, i1, i2 are indcies
void Sphere::_Subdivided(uint i0, uint i1, uint i2, int count)
{
	if (count > 0) 
	{
		uint ii0 = _LookUpIndex(i0, i1);
		uint ii1 = _LookUpIndex(i0, i2);
		uint ii2 = _LookUpIndex(i1, i2);
		_Subdivided(i0, ii0, ii1, count - 1);
		_Subdivided(i2, ii1, ii2, count - 1);
		_Subdivided(i1, ii2, ii0, count - 1);
		_Subdivided(ii0, ii1, ii2, count - 1);
	}
	else 
	{
		_Trianlge(i0, i1, i2);
	}
}

uint Sphere::_LookUpIndex(uint index0, uint index1)
{
	if (index0 > index1)
	{
		std::swap(index0, index1);
	}

	std::string key = std::to_string(index0) + "," + std::to_string(index1);
	if ( _edges.count(key) == 0)
	{
		_verts.push_back(_Uint(_verts[index0], _verts[index1]));
		_edges[key] = _verts.size() - 1;
	}

	return _edges[key];
}

point4 Sphere::_Uint(point4& a, point4& b)
{
	point4 p = a + b;
	float len = p.x * p.x + p.y * p.y + p.z * p.z;
	point4 t;

	if (len > DivideByZeroTolerance) 
	{
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void Sphere::_Trianlge(uint i0, uint i1, uint i2)
{
	_indices.push_back(i0);
	_indices.push_back(i1);
	_indices.push_back(i2);
}

float Sphere::GetRadius()
{
	return 0.5f;
}