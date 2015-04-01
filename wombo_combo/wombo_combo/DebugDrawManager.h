#pragma once

#include "Global.h"
#include "Mesh.h"
#include "Sphere.h"
#include "Line.h"
#include "MeshRender.h"
#include "MyTimer.h"
#include <list>
#include <map>
#include "Shader.h"
#include "Camera.h"
#include "Triangle.h"


typedef struct 
{
	bool isPrimitive;
	GLenum drawType;
	color4 color;
	float lineWidth;
	float expireAt; // in seconds
	matrix4 transform;
	bool isDepthEnabled;
	MeshRender* render;
}DebugDrawObject;


class DebugDrawManager
{
public:
	//Adds a line segment to the debug drawing queue
	void AddLine(const point4& from, 
		const point4& to, 
		color4& color, 
		float lineWidth = 1.0f, 
		float duration = 0.0f, 
		bool isDepthEnabled = true);

	//Adds an axis-aligned cross (3 lines converging at a point) to the debug drawing queue
	void AddCross(const point4& position, 
		color4& color, 
		float length,
		float duration = 0.0f, 
		bool isDepthEnabled = true);

	//Add a wireframe sphere to the debug drawing queue.
	void AddSphere(const point4& center, 
		float radius, 
		color4& color, 
		float duration = 0.0f, 
		bool isDepthEnabled = true);

	//Add a circle to the drawing queue
	void AddCircle(const point4& center, 
		const point3& planeNormal, 
		float radius, color4& color, 
		float duration = 0.0f, 
		bool isDepthEnabled = true);

	//Adds a set of cooridinte axes depicting the 
	// position and orientation of the given
	// transformation to the debug drawing queue
	void AddAxes(const matrix4& currentSpace, 
		color4& color, 
		float length, 
		float duration = 0.0f, 
		bool isDepthEnabled = true);

	//Add a wireframe triangle to the debug drawing queue
	void AddTriangle(const point4& v0,
		const point4& v1,
		const point4& v2,
		color4& color,
		float lineWidth = 1.0f,
		float duration = 0.0f,
		bool isDepthEnabled = true);

	//Adds an axis-aligned bounding box to the debug queue
	void AddAABB(const point4& minCoords,
		const point4& maxCorrds,
		color4& color,
		float lineWidth = 1.0f,
		float duration = 0.0f,
		bool isDepthEnabled = true);

	//Adds an oriented bounding box to the debug queue
	//scale is the radius of the OBB cube
	void AddOBB(const matrix4& transformCenter,
		const point3& scale,
		color4& color,
		float lineWidth = 1.0f,
		float duration = 0.0f,
		bool isDepthEnabled = true);

	//Adds a text to the debug drawing queue
	void AddString(const point4& position,
		const char* text,
		color4& color,
		float duration = 0.0f,
		bool isDepthEnabled = true);

	void Render(Camera* debugCamera, Shader* debugShader);

private:

	//To prevent code from being fragmentation, we need this to
	//store advanced mesh, e.g. cube, sphere, etc..
	std::map<const char*, MeshRender*> _nonePrimitives;

	std::list<DebugDrawObject> _debugDrawingQueue;
	MyTimer _timer;

public:
	static DebugDrawManager& getInstance()
	{
		static DebugDrawManager instance;
		return instance;
	}
private:
	DebugDrawManager();
	DebugDrawManager(DebugDrawManager const&) = delete;
	void operator=(DebugDrawManager const&) = delete;
};