#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assert.h>

#include <memory>

typedef glm::vec2 point2;

typedef glm::vec3 point3;
typedef glm::vec4 point4;

typedef glm::vec4 color4;
typedef glm::vec3 normal3;

typedef glm::mat4 matrix4;

typedef unsigned int uint;
typedef unsigned char uchar;


#define BUFFER_OFFSET(offset) (( void *)(offset))

const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);