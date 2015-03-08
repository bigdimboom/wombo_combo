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
	:_length(len), _width(width),
	_weight(50.0f)
{
}


Terrain::~Terrain()
{
	_mesh.ClearAll();
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
	_mesh.SetVertSize((uint)size);

	//only red for heightmap
	for (int z = 0; z < (int)_width; ++z)
	{
		for (int x = 0; x < (int)_length; ++x)
		{
			color = ((uchar*)img->pixels)[3 * (z * _length + x)];
			// why -0.5 ? because, height could be negtive.
			h = _weight * ((color / 255.0f) - (1.0f / 2.0f));
			_mesh.GetVerts()[z * _length + x] =
				point4(((float)x - (float)(_length / 2.0f)),
				h,
				((float)z - (float)(_width / 2.0f)), 1.0f);
			//printf("%d\n", cccc);
		}
	}

	//gen normals
	_mesh.SetNormSize((uint)size);

	for (int y = 0; y < (int)_length; ++y)
	{
		for (int x = 0; x < (int)_width; x += 3)
		{
			uchar r = ((uchar*)img->pixels)[y * _length + x];
			uchar g = ((uchar*)img->pixels)[y * _length + x + 1];
			uchar b = ((uchar*)img->pixels)[y * _length + x + 2];

			_mesh.GetNorms()[y * _length + x] = glm::normalize(normal3(r, g, b));
		}
	}

	SDL_FreeSurface(img);
}


void Terrain::GenIndicesArray()
{
	int indexSize = (_width - 1) * (_length - 1) * 6;
	_mesh.SetIdxSize(indexSize);
	int i = 0;

	for (int y = 0; y < (int)_width - 1; ++y)
	{
		for (int x = 0; x < (int)_length - 1; ++x)
		{
			_mesh.GetIndxs()[i] = y*(_length)+x; // x 
			_mesh.GetIndxs()[i + 1] = y*(_length)+x + 1; // x + 1
			_mesh.GetIndxs()[i + 2] = y*(_length)+x + 1 + _length; // x + 1 + _width
			/////////////////////////////////////////////////////////////
			_mesh.GetIndxs()[i + 3] = y*(_length)+x; // x + 1 + _width
			_mesh.GetIndxs()[i + 4] = y*(_length)+x + 1 + _length; // x + 1
			_mesh.GetIndxs()[i + 5] = y*(_length)+x + _length;


			i += 6;
		}
	}
}


void Terrain::GenNormals()
{
	_mesh.ClearNorms();
	_mesh.SetNormSize(_width*_length);

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
				_mesh.GetVerts()[curr_index];

			//upright
			if (l + 1 < (int)_length && w + 1 < (int)_width)
			{
				point4 x = _mesh.GetVerts()[w*_length + l + 1];
				point4 xz = _mesh.GetVerts()[(w + 1)*_length + l + 1];
				point4 z = _mesh.GetVerts()[(w + 1)*_length + l];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));

				counts[curr_index] += 2;
			}

			//upleft
			if (l - 1 >= 0 && w + 1 < (int)_width)
			{

				point4 y = _mesh.GetVerts()[(w + 1)*_length + l];
				point4 x = _mesh.GetVerts()[w*_length + l - 1];


				point4 a = x - currentPoint;
				point4 b = y - currentPoint;

				normalSum[curr_index] += glm::cross(normal3(b), normal3(a));

				counts[curr_index] += 1;
			}

			//downleft
			if (l - 1 >= 0 && w - 1 >= 0)
			{
				point4 x = _mesh.GetVerts()[w*_length + l - 1];
				point4 xz = _mesh.GetVerts()[(w - 1)*_length + l - 1];
				point4 z = _mesh.GetVerts()[(w - 1) *_length + l];

				normalSum[curr_index] += CalculateNorm(point3(currentPoint), point3(x), point3(xz), point3(z));
				counts[curr_index] += 2;
			}

			//downleft
			if (l + 1 < (int)_length && w - 1 >= 0)
			{
				point4 y = _mesh.GetVerts()[(w - 1)*_length + l];
				point4 x = _mesh.GetVerts()[w*_length + l + 1];

				point4 a = x - currentPoint;
				point4 b = y - currentPoint;

				normalSum[curr_index] += glm::cross(normal3(b), normal3(a));
				counts[curr_index] += 1;
			}

			//smooth the normals
			_mesh.GetNorms()[curr_index].x = normalSum[curr_index].x / counts[curr_index];
			_mesh.GetNorms()[curr_index].y = normalSum[curr_index].y / counts[curr_index];
			_mesh.GetNorms()[curr_index].z = normalSum[curr_index].z / counts[curr_index];

			//normalize it
			_mesh.GetNorms()[curr_index] = glm::normalize(_mesh.GetNorms()[curr_index]);
		}
	}

	delete[] normalSum;
	delete[] counts;
}


void Terrain::GenTextureCoords()
{
	int size = _length * _width;
	_mesh.SetUVSize(size);

	//map all index in (0,0) to (1,1) scale
	// 1. shift first
	// 2 .scale to (1,1) cocrdinate

	for (uint i = 0; i < _width; ++i)
	{
		for (uint x = 0; x < _length; ++x)
		{
			_mesh.GetUVs()[i*_length + x]
				= point2(
				(_mesh.GetVerts()[i*_length + x].x + _length / 2) / (_length),
				(_mesh.GetVerts()[i*_length + x].z + _width / 2) / (_width)
				);
		}
	}

}