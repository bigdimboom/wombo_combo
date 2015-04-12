#include "SimpleCDTest.h"

inline float squared(float v) { return v * v; }
bool doesCubeIntersectSphere(point3 C1, point3 C2, point3 S, float R)
{
	float dist_squared = R * R;
	/* assume C1 and C2 are element-wise sorted, if not, do that now */
	if (S.x < C1.x) dist_squared -= squared(S.x - C1.x);
	else if (S.x > C2.x) dist_squared -= squared(S.x - C2.x);
	if (S.y < C1.y) dist_squared -= squared(S.y - C1.y);
	else if (S.y > C2.y) dist_squared -= squared(S.y - C2.y);
	if (S.z < C1.z) dist_squared -= squared(S.z - C1.z);
	else if (S.z > C2.z) dist_squared -= squared(S.z - C2.z);
	return dist_squared > 0;
}



SimpleCDTest::SimpleCDTest(TerrainRenderable* inst)
	:_terrain(inst)
{
	_isCollide = false;
}


SimpleCDTest::~SimpleCDTest()
{
	//delete _cdObj->GetMesh();
	delete _cdObj;
}

void SimpleCDTest::SetMovingObj(point3 pos, point3 vel)
{
	_cdObj = new MovableActor(pos, vel);
	_cdObj->SetRadius(1.0f);
	//DebugDrawManager::getInstance().AddSphere(point4(_cdObj->GetPosition(), 1.0f), _cdObj->GetRadius(), color4(0.2f, 0.1f, 0.9f, 1.0f), 0.5f);
}

void SimpleCDTest::Update(float dTime)
{
	assert(_terrain);

	if (!_cdObj || _cdObj->GetVelocity() == point3(0.0f, 0.0f, 0.0f))
	{
		return;
	}

	_cdObj->Move(dTime);

	point3 collidePoint;
	if (_IsCollideWithTerrian(collidePoint))
	{
		_cdObj->SetPosition(collidePoint);
		_cdObj->SetVelocity(point3(0.0f, 0.0f, 0.0f));
		DebugDrawManager::getInstance().AddSphere(point4(_cdObj->GetPosition(), 1.0f), _cdObj->GetRadius(), color4(1.0f, 0.0f, 0.0f, 1.0f), 1000.0f, false);
		return;
	}

	DebugDrawManager::getInstance().AddSphere(point4(_cdObj->GetPosition(), 1.0f), _cdObj->GetRadius(), color4(0.2f, 0.1f, 0.9f, 1.0f), 100.0f);

	return;
}

bool SimpleCDTest::_IsCollideWithTerrian(point3& cdPos)
{
	_Traverse(_terrain->_octree.GetRoot(), cdPos);

	if (_isCollide)
	{
		return true;
	}
	return false;
}

bool SimpleCDTest::_IsCollideWithOctant(OctantPtr ptr)
{
	point3 nearClip[NUM_CONNERS];
	point3 farClip[NUM_CONNERS];
	Plane planes[NumPlanes];

	farClip[UP_LEFT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 0);
	farClip[UP_RIGHT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 1);
	farClip[BOTTOM_RIGHT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 2);
	farClip[BOTTOM_LEFT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 3);
	nearClip[UP_LEFT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 4);
	nearClip[UP_RIGHT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 5);
	nearClip[BOTTOM_RIGHT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 6);
	nearClip[BOTTOM_LEFT] = Plane::GetPointFromCube(ptr->center, ptr->radius, 7);

	//////////////////////////////////////////////////////////////////////////////////////////////
	planes[NEARD].Set(nearClip[UP_LEFT], nearClip[UP_RIGHT], nearClip[BOTTOM_RIGHT]);
	planes[FARD].Set(farClip[UP_RIGHT], farClip[UP_LEFT], farClip[BOTTOM_LEFT]);
	planes[LEFT].Set(nearClip[UP_LEFT], nearClip[BOTTOM_LEFT], farClip[BOTTOM_LEFT]);
	planes[RIGHT].Set(nearClip[BOTTOM_RIGHT], nearClip[UP_RIGHT], farClip[BOTTOM_RIGHT]);
	planes[TOP].Set(nearClip[UP_RIGHT], nearClip[UP_LEFT], farClip[UP_LEFT]);
	planes[BOTTOM].Set(nearClip[BOTTOM_LEFT], nearClip[BOTTOM_RIGHT], farClip[BOTTOM_RIGHT]);

	if (_IsInsideOctant(planes) || _IsIntersectOctant(planes))
	{
		return true;
	}

	return false;
}

bool SimpleCDTest::_IsInsideOctant(Plane* octantPlanes)
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if ( !octantPlanes[i].IsSphereInside(
			_cdObj->GetPosition(), _cdObj->GetRadius()) )
		{
			return false;
		}
	}
	return true;
}


bool SimpleCDTest::_IsIntersectOctant(Plane* octantPlanes)
{
	bool isOutsidePlane[NumPlanes];
	for (int i = 0; i < NumPlanes; ++i)
	{
		isOutsidePlane[i] = !octantPlanes[i].IsSphereOutside(
			_cdObj->GetPosition(), _cdObj->GetRadius());
	}

	if (octantPlanes[TOP].IsSphereIntersect(_cdObj->GetPosition(), _cdObj->GetRadius()) &&
		isOutsidePlane[LEFT] && isOutsidePlane[RIGHT] && isOutsidePlane[FARD] && isOutsidePlane[NEARD])
	{
		return true;
	}
	//
	if (octantPlanes[BOTTOM].IsSphereIntersect(_cdObj->GetPosition(), _cdObj->GetRadius()) &&
		isOutsidePlane[LEFT] && isOutsidePlane[RIGHT] && isOutsidePlane[FARD] && isOutsidePlane[NEARD])
	{
		return true;
	}
	//
	if (octantPlanes[LEFT].IsSphereIntersect(_cdObj->GetPosition(), _cdObj->GetRadius()) &&
		isOutsidePlane[TOP] && isOutsidePlane[BOTTOM] && isOutsidePlane[FARD] && isOutsidePlane[NEARD])
	{
		return true;
	}
	//
	if (octantPlanes[RIGHT].IsSphereIntersect(_cdObj->GetPosition(), _cdObj->GetRadius()) &&
		isOutsidePlane[TOP] && isOutsidePlane[BOTTOM] && isOutsidePlane[FARD] && isOutsidePlane[NEARD])
	{
		return true;
	}
	//
	if (octantPlanes[NEARD].IsSphereIntersect(_cdObj->GetPosition(), _cdObj->GetRadius()) &&
		isOutsidePlane[LEFT] && isOutsidePlane[RIGHT] && isOutsidePlane[TOP] && isOutsidePlane[BOTTOM])
	{
		return true;
	}

	if (octantPlanes[FARD].IsSphereIntersect(_cdObj->GetPosition(), _cdObj->GetRadius()) &&
		isOutsidePlane[LEFT] && isOutsidePlane[RIGHT] && isOutsidePlane[TOP] && isOutsidePlane[BOTTOM])
	{
		return true;
	}

	return false;
}


void SimpleCDTest::_Traverse(OctantPtr ptr, point3& cdPos)
{
	if (ptr == nullptr || !_IsCollideWithOctant(ptr))
	{
		return;
	}

	if (_terrain->_octree.IsLeafNode(ptr))
	{
		if (ptr->indices.size() == 0)
		{
			return;
		}
		else
		{
			for (uint i = 0; i < ptr->indices.size(); i += 3)
			{
				point4 *p0 = &_terrain->GetRawTerrain()->GetMesh().GetVerts()[i];
				point4 *p1 = &_terrain->GetRawTerrain()->GetMesh().GetVerts()[i + 1];
				point4 *p2 = &_terrain->GetRawTerrain()->GetMesh().GetVerts()[i + 2];
				if (_IsCollideWithTriangle(ptr, p0, p1, p2, cdPos))
				{
					_isCollide = true;
					return;
				}
			}
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		_Traverse(ptr->child[i], cdPos);
	}
}

bool SimpleCDTest::_IsCollideWithTriangle(OctantPtr ptr, point4* p0, point4* p1, point4* p2, point3& cdPos)
{
	Plane plane;
	//The plane normal must be pointing to the inside of terrain
	point3 pp0 = point3(*p0);
	point3 pp1 = point3(*p1);
	point3 pp2 = point3(*p2);

	plane.Set(pp0, pp1, pp2);

	if (!plane.IsSphereInside(_cdObj->GetPosition(), _cdObj->GetRadius()))
	{
		return false;
	}

	point3 projectPoint = plane.ProjPointFrom(ptr->center);

	////Check Each point
	if (glm::dot(pp1 - pp0, projectPoint - pp0) < 0)
	{
		return false;
	}

	if (glm::dot(pp0 - pp1, projectPoint - pp1) < 0)
	{
		return false;
	}

	if (glm::dot(pp1 - pp2, projectPoint - pp2) < 0)
	{
		return false;
	}

	cdPos = projectPoint;

	return true;
}