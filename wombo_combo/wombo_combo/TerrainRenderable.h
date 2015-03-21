#pragma once
#include "Terrain.h"
#include "IRenderable.h"
#include "Texture.h"
#include "Global.h"
#include <vector>
#include <string>

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
private:
	Terrain* _terrain;
	GLuint _vao, _vbo, _ebo;
	bool _isOutsideInstance;
	std::vector<std::pair<Texture*, std::pair<Path, Name> > > _textures;
	void _InitMembers();
};

