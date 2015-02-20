#include "Terrian.h"


Terrian::Terrian(int len, int width)
	:_terrian(nullptr),
	_normals(nullptr),
	_textureCoords(nullptr),
	_length(len), _width(width),
	_weight(250.0f)
{
}


Terrian::~Terrian()
{
	delete _terrian;
	delete _normals;
	delete _textureCoords;
}

void Terrian::GenHightMap(const char* hightmap)
{
	SDL_Surface* img = IMG_Load(hightmap);
	if (!img)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
		return;
	}

	uchar color = 0;
	float h = 0.0f;

	uint size = _length * _width;
	_terrian = new point4[size];

	//complexityO(n^2), uisng GPU maybe?
	//only red for heightmap
	for (uint y = 0; y <(uint)_length; ++y){
		for (uint x = 0; x < (uint)_width; ++x){
			color = ((uchar*)img->pixels)[3*(y * _length + x)];
			// why -0.5 ? because, height could be negtive.
			h = _weight * ((color / 255.0f) - 0.5f);
			_terrian[y * _length + x] =
				point4((float)x - 512.0f, h, 512.0f - (float)y, 1.0f);
			//printf("%d\n", cccc);
		}
	}

	SDL_FreeSurface(img);

	//GenNormals();
}

//point3* Terrian::GetNormals() const
//{
//	
//}
//
//point2* Terrian::GetUVCoords() const
//{
//
//}