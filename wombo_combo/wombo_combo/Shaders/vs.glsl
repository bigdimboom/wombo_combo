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
out vec3 my_normal;
out vec3 my_point;
/////////////////////////////


void main()
{
    my_uv = uv;
	my_point = (view * model * position).xyz;
	my_normal = normal;
    gl_Position = projection * view  * model *position;
}


