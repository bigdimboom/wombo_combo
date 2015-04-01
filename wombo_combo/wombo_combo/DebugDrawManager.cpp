#include "DebugDrawManager.h"

DebugDrawManager::DebugDrawManager()
{
	_nonePrimitives["sphere"] = new MeshRender();
	_nonePrimitives["sphere"]->Init(new Sphere, GL_TRIANGLES);
	if (_nonePrimitives.count("sphere") == 0)
	{
		std::cout << "error init sphere\n";
		system("PAUSE");
	}
};

//Adds a line segment to the debug drawing queue
void DebugDrawManager::AddLine(const point4& from,
	const point4& to,
	color4& color,
	float lineWidth,
	float duration,
	bool isDepthEnabled)
{
	MeshRender* lineMesh = new MeshRender();
	lineMesh->Init(new Line(from, to), GL_LINES);
	DebugDrawObject line;
	line.isPrimitive = true;
	line.drawType = GL_LINES;
	line.color = color;
	line.lineWidth = lineWidth;
	line.expireAt = _timer.GetElapsedTime() + 1000 * duration;
	line.transform = matrix4(1.0f);
	line.isDepthEnabled = isDepthEnabled;
	line.render = lineMesh;
}


//Adds an axis-aligned cross (3 lines converging at a point) to the debug drawing queue
void DebugDrawManager::AddCross(const point4& position,
	color4& color,
	float length,
	float duration,
	bool isDepthEnabled)
{

}


//Add a wireframe sphere to the debug drawing queue.
void DebugDrawManager::AddSphere(const point4& center,
	float radius,
	color4& color,
	float duration,
	bool isDepthEnabled)
{
	DebugDrawObject sphere;
	sphere.isPrimitive = false;
	sphere.drawType = GL_TRIANGLES;
	sphere.color = point4(1.0f, 0.0f, 0.0f, 1.0f);
	sphere.lineWidth = 1.0f;
	sphere.expireAt = _timer.GetElapsedTime() + duration * 1000;
	float scale = radius / 0.5f;
	sphere.transform = glm::translate(matrix4(1.0), point3(center)) *
		glm::scale(matrix4(1.0), point3(scale, scale, scale));
	sphere.isDepthEnabled = isDepthEnabled;
	sphere.render = _nonePrimitives["sphere"];
	//put sphere in the drawing queue
	_debugDrawingQueue.push_back(sphere);
}


//Add a circle to the drawing queue
void DebugDrawManager::AddCircle(const point4& center,
	const point3& planeNormal,
	float radius, color4& color,
	float duration,
	bool isDepthEnabled)
{

}



//Adds a set of cooridinte axes depicting the 
// position and orientation of the given
// transformation to the debug drawing queue
void DebugDrawManager::AddAxes(const matrix4& currentSpace,
	color4& color,
	float length,
	float duration,
	bool isDepthEnabled)
{

}

//Add a wireframe triangle to the debug drawing queue
void DebugDrawManager::AddTriangle(const point4& v0,
	const point4& v1,
	const point4& v2,
	color4& color,
	float lineWidth,
	float duration,
	bool isDepthEnabled)
{
	MeshRender* triangleMesh = new MeshRender();
	triangleMesh->Init(new Triangle(v0, v1, v2), GL_TRIANGLES);
	DebugDrawObject triangle;
	triangle.isPrimitive = true;
	triangle.drawType = GL_TRIANGLES;
	triangle.color = color;
	triangle.lineWidth = lineWidth;
	triangle.expireAt = _timer.GetElapsedTime() + 1000 * duration;
	triangle.transform = matrix4(1.0f);
	triangle.isDepthEnabled = isDepthEnabled;
	triangle.render = triangleMesh;
}


//Adds an axis-aligned bounding box to the debug queue
void DebugDrawManager::AddAABB(const point4& minCoords,
	const point4& maxCorrds,
	color4& color,
	float lineWidth,
	float duration,
	bool isDepthEnabled)
{

}


//Adds an oriented bounding box to the debug queue
//scale is the radius of the OBB cube
void DebugDrawManager::AddOBB(const matrix4& centerTransform,
	const point3& scale,
	color4& color,
	float lineWidth,
	float duration,
	bool isDepthEnabled)
{

}


//Adds a text to the debug drawing queue
void DebugDrawManager::AddString(const point4& position,
	const char* text,
	color4& color,
	float duration,
	bool isDepthEnabled)
{

}