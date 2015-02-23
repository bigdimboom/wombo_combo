#pragma once

#include "Global.h"
#include <SDL_image.h>

class Texture
{
public:
	Texture();
	~Texture();
	bool Load(const char* img_path, bool isMipMaps = false);
	void Bind(int textureUnit=0);
	void Release();
private:
	bool _isMipMapsRequested;
	GLuint _texture;
	GLuint _sampler;
private:
	GLenum _Convert(SDL_PixelFormat* sdl_img_format);
};

