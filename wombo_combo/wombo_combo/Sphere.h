#pragma once
#include "Mesh.h"
#include <vector>
#include <unordered_map>

#define LOD 4

class Sphere :
	public Mesh
{
public:
	Sphere();
	~Sphere();
	void Build();
	float GetRadius();
private:
	void _Subdivided(uint i0, uint i1, uint i2, int count); //Done
	void _Tetrahedron(); // Done
	void _Trianlge(uint i0, uint i1, uint i2); //Triangle
	point4 _Uint(point4& a, point4& b); //Done
	uint _LookUpIndex(uint index0, uint index1); // Done

	//void _GouraudOptimize();
	//void _OptimizeNormal();

	std::unordered_map<std::string, uint> _edges;
	std::vector<point4> _verts;
	std::vector<uint> _indices;

	bool _isBuilt;
};

