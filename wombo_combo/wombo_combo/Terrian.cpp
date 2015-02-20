#include "Terrian.h"


Terrian::Terrian(int len, int width)
	:_terrian(nullptr),
	_indices(nullptr),
	_normals(nullptr),
	_textureCoords(nullptr),
	_length(len), _width(width), _index_size(0),
	_weight(8.0f)
{
}


Terrian::~Terrian()
{
	delete[] _terrian;
	delete[] _normals;
	delete[] _textureCoords;
	delete[] _indices;
}

void Terrian::GenTerrian(const char* hightmap, bool genNormals, bool genUVs)
{

	GenHeightMap(hightmap);
	GenIndicesArray();
	uint* ptr = _indices;
	if (genNormals)
	{
		//GenNormals();
	}
	if (genUVs)
	{
		//GetUVCoords()
	}
	return;
}

void Terrian::GenHeightMap(const char* hightmap)
{
	SDL_Surface* img = IMG_Load(hightmap);
	if (!img)
	{
		printf("IMG_Load: %s\n", IMG_GetError());
		return;
	}

	uchar color = 0;
	float h = 0.0f;

	int size = _length * _width;
	_terrian = new point4[size];

	//complexityO(n^2), uisng GPU maybe?
	//only red for heightmap
	for (uint y = 0; y < (uint)_length; ++y){
		for (uint x = 0; x < (uint)_width; ++x){
			color = ((uchar*)img->pixels)[3 * (y * _length + x)];
			// why -0.5 ? because, height could be negtive.
			h = _weight * ((color / 255.0f) - 0.5f);
			_terrian[y * _length + x] =
				point4(((float)x - (_length / 2)), h, ((_width / 2) - (float)y), 1.0f);
			//printf("%d\n", cccc);
		}
	}

	SDL_FreeSurface(img);
}


void Terrian::GenIndicesArray()
{
	_index_size = (_width - 1) * (_length - 1) * 6;
	_indices = new GLuint[_index_size];

	int i = 0;

	for (uint y = 0; y < (uint)_width - 1; ++y)
	{
		for (uint x = 0; x < (uint)_length - 1; ++x)
		{
			_indices[y*(_width - 1) + x + i] = y*(_width - 1) + x; // x 
			_indices[y*(_width - 1) + x + 1 + i] = y*(_width - 1) + x + 1; // x + 1
			_indices[y*(_width - 1) + x + 2 + i] = y*(_width - 1) + x + 1 + _width; // x + 1 + _width
			/////////////////////////////////////////////////////////////
			_indices[y*(_width - 1) + x + 3 + i] = y*(_width - 1) + x + 1 + _width; // x + 1 + _width
			_indices[y*(_width - 1) + x + 4 + i] = y*(_width - 1) + x + _width; // x + 1
			_indices[y*(_width - 1) + x + 5 + i] = y*(_width - 1) + x;

			i += 5;
		}
	}
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