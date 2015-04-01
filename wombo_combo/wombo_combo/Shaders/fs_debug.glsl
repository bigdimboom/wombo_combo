#version 330 core

out vec4 fragColor_out;
uniform vec4 color;

void main()
{
	fragColor_out = color;
}
