#include "Texture.h"


Texture::Texture()
	:_isMipMapsRequested(false),
	_texture(0), _sampler(0)
{
}


Texture::~Texture()
{
}

bool Texture::Load(const char* img_path, bool isMipMaps)
{
	SDL_Surface* img = IMG_Load(img_path);
	if (!img)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
		return false;
	}

	//Gen a texture object
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glGenSamplers(1, &_sampler);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//You Need to watch out for the two formats
	GLenum colorformat = _Convert(img->format);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, colorformat, GL_UNSIGNED_BYTE, img->pixels);

	if (isMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	_isMipMapsRequested = isMipMaps;

	SDL_FreeSurface(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture::Bind(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glBindSampler(textureUnit, _sampler);
}

void Texture::Release()
{
	glDeleteTextures(1, &_texture);
}

GLenum Texture::_Convert(SDL_PixelFormat* sdl_img_format)
{
	GLenum textureFormat;
	if (sdl_img_format->BytesPerPixel == 4) // contains an alpha channel
	{
		if (sdl_img_format->Rshift == 24 && sdl_img_format->Aloss == 0) textureFormat = GL_ABGR_EXT;
		else if (sdl_img_format->Rshift == 16 && sdl_img_format->Aloss == 8) textureFormat = GL_BGRA;
		else if (sdl_img_format->Rshift == 16 && sdl_img_format->Ashift == 24) textureFormat = GL_BGRA;
		else if (sdl_img_format->Rshift == 0 && sdl_img_format->Ashift == 24) textureFormat = GL_RGBA;
		else throw std::logic_error("Pixel Format not recognized for GL display");
	}
	else if (sdl_img_format->BytesPerPixel == 3) // no alpha channel
	{
		if (sdl_img_format->Rshift == 16) textureFormat = GL_BGR;
		else if (sdl_img_format->Rshift == 0) textureFormat = GL_RGB;
		else throw std::logic_error("Pixel Format not recognized for GL display");
	}

	return textureFormat;
}