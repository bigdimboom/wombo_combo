#include "Terrian.h"


static normal3 CalculateNorm(point3 curr, point3 x, point3 xz, point3 z)
{
	point3 a = x - curr;
	point3 b = xz - curr;
	normal3 n1 = glm::cross(a, b);
	a = b;
	b = z - curr;
	normal3 n2 = glm::cross(a, b);
	return n1 + n2;
}

Terrian::Terrian(int len, int width)
	:_grids(nullptr),
	_indices(nullptr),
	_normals(nullptr),
	_textureCoords(nullptr),
	_length(len), _width(width), _index_size(0),
	_weight(50.0f)
{
}


Terrian::~Terrian()
{
	//delete[] _terrian;
	//delete[] _normals;
	//delete[] _textureCoords;
	//delete[] _indices;
}

void Terrian::GenTerrian(const char* hightmap, bool genNormals, bool genUVs)
{

	GenHeightMap(hightmap);
	GenIndicesArray();
	if (genNormals)
	{
		GenNormals();
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
	_grids = std::shared_ptr<point4>(new point4[size], [](point4 *p) { delete[] p; });

	//only red for heightmap
	for (int y = 0; y < (int)_length; ++y)
	{
		for (int x = 0; x < (int)_width; ++x)
		{
			color = ((uchar*)img->pixels)[3 * (y * _length + x)];
			// why -0.5 ? because, height could be negtive.
			h = _weight * ((color / 255.0f) - 0.5f);
			_grids.get()[y * _length + x] =
				point4(((float)x - (_length / 2)), h, ((_width / 2) - (float)y), 1.0f);
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


void Terrian::GenIndicesArray()
{
	_index_size = (_width - 1) * (_length - 1) * 6;
	_indices = std::shared_ptr<uint>(new GLuint[_index_size], [](uint *p) { delete[] p; });

	int i = 0;

	for (int y = 0; y < (int)_width - 1; ++y)
	{
		for (int x = 0; x < (int)_length - 1; ++x)
		{
			_indices.get()[y*(_width - 1) + x + i] = y*(_width - 1) + x; // x 
			_indices.get()[y*(_width - 1) + x + 1 + i] = y*(_width - 1) + x + 1; // x + 1
			_indices.get()[y*(_width - 1) + x + 2 + i] = y*(_width - 1) + x + 1 + _width; // x + 1 + _width
			/////////////////////////////////////////////////////////////
			_indices.get()[y*(_width - 1) + x + 3 + i] = y*(_width - 1) + x + 1 + _width; // x + 1 + _width
			_indices.get()[y*(_width - 1) + x + 4 + i] = y*(_width - 1) + x + _width; // x + 1
			_indices.get()[y*(_width - 1) + x + 5 + i] = y*(_width - 1) + x;

			i += 5;
		}
	}
}

void Terrian::GenNormals()
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

				point4 x = _grids.get()[(w + 1)*_length + l];
				point4 xz = _grids.get()[(w + 1)*_length + l - 1];
				point4 z = _grids.get()[w *_length + l - 1];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));

				counts[curr_index] += 2;
			}

			if (l - 1 >= 0 && w - 1 >= 0)
			{
				point4 x = _grids.get()[w*_length + l - 1];
				point4 xz = _grids.get()[(w - 1)*_length + l - 1];
				point4 z = _grids.get()[(w - 1) *_length + l];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));
				counts[curr_index] += 2;
			}

			if (l + 1 < (int)_length && w - 1 >= 0)
			{
				point4 x = _grids.get()[(w - 1)*_length + l];
				point4 xz = _grids.get()[(w - 1)*_length + l + 1];
				point4 z = _grids.get()[w*_length + l + 1];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));
				counts[curr_index] += 2;
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