#include "DebugDrawManager.h"

DebugDrawManager::DebugDrawManager()
{
	_nonePrimitives[SPHERE] = new MeshRender();
	_nonePrimitives[SPHERE]->Init(new Sphere, GL_TRIANGLES);
	if (_nonePrimitives[SPHERE] == nullptr)
	{
		std::cout << "error init sphere\n";
		system("PAUSE");
	}
	_timer.Start();
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
	//line.drawType = GL_LINES;
	line.color = color;
	line.lineWidth = lineWidth;
	line.expireAt = _timer.GetElapsedTime() + 1000 * duration;
	line.transform = matrix4(1.0f);
	line.isDepthEnabled = isDepthEnabled;
	line.render = lineMesh;
	_debugDrawingQueue.push_back(line);
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
	//sphere.drawType = GL_TRIANGLES;
	sphere.color =color;
	sphere.lineWidth = 1.0f;
	sphere.expireAt = _timer.GetElapsedTime() + duration * 1000;
	float scale = radius / 0.5f;
	sphere.transform = glm::translate(matrix4(1.0), point3(center)) *
		glm::scale(matrix4(1.0), point3(scale, scale, scale));
	sphere.isDepthEnabled = isDepthEnabled;
	sphere.render = _nonePrimitives[SPHERE];
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
	//triangle.drawType = GL_TRIANGLES;
	triangle.color = color;
	triangle.lineWidth = lineWidth;
	triangle.expireAt = _timer.GetElapsedTime() + 1000 * duration;
	triangle.transform = matrix4(1.0f);
	triangle.isDepthEnabled = isDepthEnabled;
	triangle.render = triangleMesh;
	_debugDrawingQueue.push_back(triangle);
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

void DebugDrawManager::EnableWorldPlane(
	color4& color,
	int size,
	float lineWidth,
	float duration,
	bool isDepthEnabled)
{
	static int worldSize = 0;

	if (worldSize != size)
	{
		worldSize = size;
		MeshRender* lineMesh = new MeshRender();
		lineMesh->Init(new Grid(size), GL_LINES);
		if (_nonePrimitives[WORLD_GRID] != nullptr)
		{
			std::cout << "You can only draw world grid once.\n";
			delete _nonePrimitives[WORLD_GRID];
		}
		_nonePrimitives[WORLD_GRID] = lineMesh;
	}

	DebugDrawObject worldPlane;
	worldPlane.isPrimitive = false;
	//worldPlane.drawType = GL_LINES;
	worldPlane.color = color;
	worldPlane.lineWidth = lineWidth;
	worldPlane.expireAt = _timer.GetElapsedTime() + 1000 * duration;
	worldPlane.transform = matrix4(1.0f);
	worldPlane.isDepthEnabled = isDepthEnabled;
	worldPlane.render = _nonePrimitives[WORLD_GRID];
	_debugDrawingQueue.push_back(worldPlane);
}

//Adds a text to the debug drawing queue
void DebugDrawManager::AddGrid(matrix4& transform,
	color4& color,
	int sizex,
	int sizez,
	float lineWidth,
	float duration,
	bool isDepthEnabled)
{
	MeshRender* gridMesh = new MeshRender();
	gridMesh->Init(new Grid(sizex, sizez), GL_LINES);
	DebugDrawObject grid;
	grid.isPrimitive = true;
	//grid.drawType = GL_LINES;
	grid.color = color;
	grid.lineWidth = lineWidth;
	grid.expireAt = _timer.GetElapsedTime() + 1000 * duration;
	grid.transform = transform;
	grid.isDepthEnabled = isDepthEnabled;
	grid.render = gridMesh;
	_debugDrawingQueue.push_back(grid);
}

void DebugDrawManager::Render(Camera* debugCamera, Shader* debugShader)
{
	_timer.Stop();
	DrawIterator itr;
	for (itr = _debugDrawingQueue.begin(); itr != _debugDrawingQueue.end();)
	{
		itr->render->Render(debugCamera, debugShader,
			itr->color, itr->transform, itr->lineWidth, itr->isDepthEnabled);

		if (_timer.GetElapsedTime() >= itr->expireAt)
		{
			if (itr->isPrimitive)
			{
				delete itr->render;
			}
			//watch out: the iterator usage
			itr = _debugDrawingQueue.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}