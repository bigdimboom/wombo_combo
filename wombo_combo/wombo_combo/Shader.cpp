#include "Shader.h"

Shader::Shader()
	:_program(0)
{
}


Shader::~Shader()
{
}


GLuint Shader::CreateShader(GLenum eShaderType, const char *strShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string source;

	// Open files
	std::ifstream shaderFile(strShaderFile);
	if (shaderFile.fail() || !shaderFile.is_open())
	{
		std::cout << strShaderFile << "Open Failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::stringstream stream;
	// Read file's buffer contents into streams
	stream << shaderFile.rdbuf();
	// close file handlers
	shaderFile.close();
	// Convert stream into string
	source = stream.str();

	puts(source.c_str());

	GLuint shader = glCreateShader(eShaderType);
	const char *ss = source.c_str();

	glShaderSource(shader, 1, &ss, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar strInfoLog[4096];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		char strShaderType[16];
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: sprintf_s(strShaderType, "vertex"); break;
		case GL_GEOMETRY_SHADER: sprintf_s(strShaderType, "geometry"); break;
		case GL_FRAGMENT_SHADER: sprintf_s(strShaderType, "fragment"); break;
		}

		printf("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);

		glDeleteShader(shader);
		return -1;
	}
	else
		puts("Shader compiled sucessfully!");

	return shader;
}


bool Shader::Comiple(const char *vsPath, const char *fsPath)
{
	GLuint vertexShader;
	GLuint fragmentShader;

	vertexShader = CreateShader(GL_VERTEX_SHADER, vsPath);
	fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fsPath);

	/* So we've compiled our shaders, now we need to link them in to the program
	that will be used for rendering. */

	/*This section could be broken out into a separate function, but we're doing it here
	because I'm not using C++ STL features that would make this easier. Tutorial doing so is
	here: http://www.arcsynthesis.org/gltut/Basics/Tut01%20Making%20Shaders.html */

	_program = glCreateProgram();

	glAttachShader(_program, vertexShader);
	glAttachShader(_program, fragmentShader);

	glLinkProgram(_program); //linking!

	//error checking
	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar strInfoLog[4096];
		glGetProgramInfoLog(_program, infoLogLength, NULL, strInfoLog);
		printf("Shader linker failure: %s\n", strInfoLog);
		return false;
	}
	else
		puts("Shader linked sucessfully!");

	glDetachShader(_program, vertexShader);
	glDetachShader(_program, fragmentShader);

	if (_program == -1)
	{
		std::cout << "Unable to Compile the Shader\n";
		return false;
	}

	return true;
}

bool Shader::Comiple(const char *vsPath, const char *gsPath, const char *fsPath)
{
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;

	vertexShader = CreateShader(GL_VERTEX_SHADER, vsPath);
	geometryShader = CreateShader(GL_GEOMETRY_SHADER, gsPath);
	fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fsPath);

	/* So we've compiled our shaders, now we need to link them in to the program
	that will be used for rendering. */

	/*This section could be broken out into a separate function, but we're doing it here
	because I'm not using C++ STL features that would make this easier. Tutorial doing so is
	here: http://www.arcsynthesis.org/gltut/Basics/Tut01%20Making%20Shaders.html */

	_program = glCreateProgram();

	glAttachShader(_program, vertexShader);
	glAttachShader(_program, geometryShader);
	glAttachShader(_program, fragmentShader);

	glLinkProgram(_program); //linking!

	//error checking
	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar strInfoLog[4096];
		glGetProgramInfoLog(_program, infoLogLength, NULL, strInfoLog);
		printf("Shader linker failure: %s\n", strInfoLog);
		return false;
	}
	else
		puts("Shader linked sucessfully!");

	glDetachShader(_program, vertexShader);
	glDetachShader(_program, geometryShader);
	glDetachShader(_program, fragmentShader);

	if (_program == -1)
	{
		std::cout << "Unable to Compile the Shader\n";
		return false;
	}

	return true;
}

void Shader::Delete()
{
	assert(_program > 0);
	glDeleteProgram(_program);
}

