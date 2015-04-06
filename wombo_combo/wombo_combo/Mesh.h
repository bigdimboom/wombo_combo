#pragma once
#include "Global.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	inline point4* GetVerts() const { return _verts; }
	inline uint* GetIndxs() const { return _indices; }
	inline normal3* GetNorms() const { return _normals; }
	inline point2* GetUVs() const { return _uv; }

	void SetVertSize(uint vsize);
	void SetNormSize(uint nsize);
	void SetUVSize(uint uvsize);
	void SetIdxSize(uint isize);

	inline uint GetVSize() const { return _vertSize; }
	inline uint GetIdxSize() const { return _indexSize; }
	inline uint GetNormSize() const { return _normalSize; }
	inline uint GetUVSize() const { return _uvSize; }

	inline uint GetVSizeInBytes() const { return _vertSize * sizeof(point4); }
	inline uint GetIdxSizeInBytes() const { return _indexSize * sizeof(uint); }
	inline uint GetNormSizeInBytes() const { return _normalSize * sizeof(normal3); }
	inline uint GetUVSizeInBytes() const { return _uvSize * sizeof(point2); }

	void ClearVerts();
	void ClearNorms();
	void ClearIdxs();
	void ClearUVs();

	void ClearAll();

	void OptimizeNorm();

private:
	uint _vertSize;
	uint _normalSize;
	uint _indexSize;
	uint _uvSize;
	point4* _verts;
	normal3* _normals;
	point2* _uv;
	uint* _indices;
};

