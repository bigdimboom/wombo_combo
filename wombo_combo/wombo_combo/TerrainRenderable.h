#pragma once
#include "Terrain.h"
#include "IRenderable.h"
#include "Texture.h"
#include "Global.h"
#include <vector>
#include <string>
#include "Shader.h"
#include "Frustum.h"
#include "Octree.h"

#define LAYERS 3
#define NUM_TRIANGLES 600

typedef const char* Path;
typedef const char* Name;

class TerrainRenderable : public IRenderable
{
public:
	TerrainRenderable();
	TerrainRenderable(Terrain* inst);
	TerrainRenderable(int width, int size);
	~TerrainRenderable();
	void Init();
	void Render(Camera* cam, point3* lightPos, Shader* shader) override;
	inline Terrain* GetRawTerrain() const { return _terrain; }
	void AttachTexture(Path, Name);
	void EnableCull(bool isCull) { _isCull = isCull; }
	void EnableDebug(bool isDebug) { _isDebug = isDebug; }
	Frustum _frustum;
	Octree _octree;
private:
	Terrain* _terrain;
	GLuint _vao, _vbo, _ebo;
	bool _isOutsideInstance;
	std::vector<std::pair<Texture*, std::pair<Path, Name> > > _textures;
	void _InitMembers();
private:
	bool _isDebug;
	bool _isCull;
	void _Cull(OctantPtr ptr);
	std::vector<std::vector<uint>*> _idx;
};

