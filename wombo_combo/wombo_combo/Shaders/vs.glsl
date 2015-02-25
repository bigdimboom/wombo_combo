#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
//////////////////////////////
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//////////////////////////////
out vec2 my_uv;
/////////////////////////////


void main()
{
    my_uv = uv;
    gl_Position = projection * view  * position;
}

