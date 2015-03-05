#include "Terrain.h"


static normal3 CalculateNorm(point3 curr, point3 x, point3 xz, point3 z)
{
	point3 a = x - curr;
	point3 b = xz - curr;
	normal3 n1 = glm::cross(a, b);
	a = b;
	b = z - curr;
	normal3 n2 = glm::cross(a, b);
	return glm::normalize(n1 + n2);
}

Terrain::Terrain(int len, int width)
	:_grids(nullptr),
	_indices(nullptr),
	_normals(nullptr),
	_textureCoords(nullptr),
	_length(len), _width(width), _index_size(0),
	_weight(100.0f)
{
}


Terrain::~Terrain()
{
	//delete[] _terrian;
	//delete[] _normals;
	//delete[] _textureCoords;
	//delete[] _indices;
}

void Terrain::GenTerrian(const char* hightmap, bool genNormals, bool genUVs)
{

	GenHeightMap(hightmap);
	GenIndicesArray();
	if (genNormals)
	{
		GenNormals();
	}
	if (genUVs)
	{
		GenTextureCoords();
	}
	return;
}

void Terrain::GenHeightMap(const char* hightmap)
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
	_grids = std::shared_ptr<point4>(new point4[size], [](point4 *p) { delete[] p; });

	//only red for heightmap
	for (int z = 0; z < (int)_width; ++z)
	{
		for (int x = 0; x < (int)_length; ++x)
		{
			color = ((uchar*)img->pixels)[3 * (z * _length + x)];
			// why -0.5 ? because, height could be negtive.
			h = _weight * ((color / 255.0f) - (1.0f/2.0f));
			_grids.get()[z * _length + x] =
				point4(((float)x - (float)(_length / 2.0f)), h, ((float)z - (float)(_width / 2.0f)), 1.0f);
			//printf("%d\n", cccc);
		}
	}

	//gen normals
	_normals = std::shared_ptr<normal3>(new normal3[size], [](normal3* p) { delete[] p; });
	for (int y = 0; y < (int)_length; ++y)
	{
		for (int x = 0; x < (int)_width; x += 3)
		{
			uchar r = ((uchar*)img->pixels)[y * _length + x];
			uchar g = ((uchar*)img->pixels)[y * _length + x + 1];
			uchar b = ((uchar*)img->pixels)[y * _length + x + 2];

			_normals.get()[y * _length + x] = glm::normalize(normal3(r, g, b));
		}
	}

	SDL_FreeSurface(img);
}


void Terrain::GenIndicesArray()
{
	_index_size = (_width - 1) * (_length - 1) * 6;
	_indices = std::shared_ptr<uint>(new GLuint[_index_size], [](uint *p) { delete[] p; });

	int i = 0;

	for (int y = 0; y < (int)_width - 1; ++y)
	{
		for (int x = 0; x < (int)_length - 1; ++x)
		{
			//_indices.get()[y*(_length - 1) + x + i] = y*(_length - 1) + x; // x 
			//_indices.get()[y*(_length - 1) + x + i + 1] = y*(_length - 1) + x + 1; // x + 1
			//_indices.get()[y*(_length - 1) + x + i + 2] = y*(_length - 1) + x + 1 + _length; // x + 1 + _width
			///////////////////////////////////////////////////////////////
			//_indices.get()[y*(_length - 1) + x + i + 3] = y*(_length - 1) + x; // x + 1 + _width
			//_indices.get()[y*(_length - 1) + x + i + 4] = y*(_length - 1) + x + 1 + _length; // x + 1
			//_indices.get()[y*(_length - 1) + x + i + 5] = y*(_length - 1) + x + _length;

			_indices.get()[i] = y*(_length) + x; // x 
			_indices.get()[i + 1] = y*(_length) + x + 1; // x + 1
			_indices.get()[i + 2] = y*(_length) + x + 1 + _length; // x + 1 + _width
			/////////////////////////////////////////////////////////////
			_indices.get()[i + 3] = y*(_length) + x; // x + 1 + _width
			_indices.get()[i + 4] = y*(_length) + x + 1 + _length; // x + 1
			_indices.get()[i + 5] = y*(_length) + x + _length;


			i += 6;
		}
	}
}

void Terrain::GenNormals()
{
	_normals.reset();
	_normals = std::shared_ptr<normal3>(
		new normal3[_width*_length], [](normal3* p) { delete[] p; }
	);

	normal3* normalSum = new normal3[_width*_length];
	int* counts = new int[_width*_length];
	int curr_index;
	point4 currentPoint;

	//Next time don't use l and 1 together
	for (int w = 0; w < (int)_width; ++w)
	{
		for (int l = 0; l < (int)_length; ++l)
		{
			curr_index = w*_length + l;
			currentPoint =
				_grids.get()[curr_index];

			//upright
			if (l + 1 < (int)_length && w + 1 < (int)_width)
			{
				point4 x = _grids.get()[w*_length + l + 1];
				point4 xz = _grids.get()[(w + 1)*_length + l + 1];
				point4 z = _grids.get()[(w + 1)*_length + l];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));

				counts[curr_index] += 2;
			}

			//upleft
			if (l - 1 >= 0 && w + 1 < (int)_width)
			{

				point4 y = _grids.get()[(w + 1)*_length + l];
				point4 x = _grids.get()[ w*_length + l - 1];

				
				point4 a = x - currentPoint;
				point4 b = y - currentPoint;

				normalSum[curr_index] += glm::cross(normal3(b), normal3(a));

				counts[curr_index] += 1;
			}

			//downleft
			if (l - 1 >= 0 && w - 1 >= 0)
			{
				point4 x = _grids.get()[w*_length + l - 1];
				point4 xz = _grids.get()[(w - 1)*_length + l - 1];
				point4 z = _grids.get()[(w - 1) *_length + l];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));
				counts[curr_index] += 2;
			}

			//downleft
			if (l + 1 < (int)_length && w - 1 >= 0)
			{
				point4 y = _grids.get()[(w - 1)*_length + l];
				point4 x = _grids.get()[w*_length + l + 1];

				point4 a = x - currentPoint;
				point4 b = y - currentPoint;

				normalSum[curr_index] += glm::cross(normal3(b), normal3(a));
				counts[curr_index] += 1;
			}

			//smooth the normals
			_normals.get()[curr_index].x = normalSum[curr_index].x / counts[curr_index];
			_normals.get()[curr_index].y = normalSum[curr_index].y / counts[curr_index];
			_normals.get()[curr_index].z = normalSum[curr_index].z / counts[curr_index];

			//normalize it
			_normals.get()[curr_index] = glm::normalize(_normals.get()[curr_index]);
		}
	}

	delete[] normalSum;
	delete[] counts;
}

void Terrain::GenTextureCoords()
{
	int size = _length * _width;
	_textureCoords = std::shared_ptr<point2>(
		new point2[size], [](point2* p){ delete[] p; }
		);

	//map all index in (0,0) to (1,1) scale
	// 1. shift first
	// 2 .scale to (1,1) cocrdinate

	for (uint i = 0; i < _width; ++i)
	{
		for (uint x = 0; x < _length; ++x)
		{
			_textureCoords.get()[i*_length + x]
				= point2(
				(_grids.get()[i*_length + x].x + _length / 2) / (_length ),
				(_grids.get()[i*_length + x].z + _width / 2) / (_width)
				);
		}
	}

}