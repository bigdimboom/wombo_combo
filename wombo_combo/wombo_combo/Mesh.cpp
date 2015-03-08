#include "Mesh.h"


Mesh::Mesh()
	:_vertSize(0),
	_normalSize(0),
	_indexSize(0),
	_uvSize(0),
	_verts(nullptr),
	_normals(nullptr),
	_uv(nullptr),
	_indices(nullptr)
{
}


Mesh::~Mesh()
{
	ClearAll();
}

void Mesh::SetVertSize(uint vsize)
{
	ClearVerts();
	_verts = new point4[vsize];
	_vertSize = vsize;
}

void Mesh::SetNormSize(uint nsize)
{
	ClearNorms();
	_normals = new normal3[nsize];
	_normalSize = nsize;
}

void Mesh::SetUVSize(uint uvsize)
{
	ClearUVs();
	_uv = new point2[uvsize];
	_uvSize = uvsize;
}

void Mesh::SetIdxSize(uint isize)
{
	ClearIdxs();
	_indices = new uint[isize];
	_indexSize = isize;
}

void Mesh::ClearAll()
{
	ClearVerts();
	ClearNorms();
	ClearIdxs();
	ClearUVs();
}

void Mesh::ClearVerts()
{
	if (_verts != nullptr){
		delete[] _verts;
		_vertSize = 0;
		_verts = nullptr;
	}
}

void Mesh::ClearNorms()
{
	if (_normals != nullptr)
	{
		delete[] _normals;
		_normalSize = 0;
		_normals = nullptr;
	}
}

void Mesh::ClearIdxs()
{
	if (_indices != nullptr)
	{
		delete[] _indices;
		_indexSize = 0;
		_indices = nullptr;
	}
}

void Mesh::ClearUVs()
{
	if (_uv != nullptr)
	{
		delete[] _uv;
		_uvSize = 0;
		_uv = nullptr;
	}
}

void Mesh::OptimizeNorm()
{
	assert(_normals != nullptr);
	assert(_verts != nullptr);

}