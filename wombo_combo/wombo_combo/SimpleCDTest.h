#pragma once
#include "Global.h"
#include "Sphere.h"
#include "Octree.h"
#include "TerrainRenderable.h"
#include "MovableActor.h"
#include "MeshRender.h"
#include "Plane.h"
#include "DebugDrawManager.h"
#include <glm/gtx/intersect.hpp>


typedef MovableActor PhxActor;

class SimpleCDTest
{
public:
	SimpleCDTest(TerrainRenderable* inst);
	void SetMovingObj(point3 pos, point3 vel);
	void Update(float dTime);
	~SimpleCDTest();
private:
	bool _isCollide;
	TerrainRenderable* _terrain;
	PhxActor* _cdObj;
	bool _IsCollideWithTerrian(point3& cdPos);
	bool _IsCollideWithOctant(OctantPtr ptr);
	bool _IsInsideOctant(Plane* octantPlanes);
	bool _IsIntersectOctant(Plane* octantPlanes);

	void _Traverse(OctantPtr ptr, point3& cdPos);
	bool _IsCollideWithTriangle(OctantPtr ptr, point4* p0, point4* p1, point4* p2, point3& cdPos);
	enum Side
	{
		NEARD = 0,
		FARD,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NumPlanes
	};

	enum
	{
		UP_LEFT = 0,
		UP_RIGHT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT,
		NUM_CONNERS
	};
};

