#version 330

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 velocity; 
layout (location = 2) in vec3 acceleration; 
layout (location = 3) in vec4 color; 
layout (location = 4) in float life; 
layout (location = 5) in float size; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;

vec3 GetPosition()
{
	return position + velocity * life + 0.5 * acceleration * life * life;
}

void main()
{
	gl_Position = projection * view  * model * vec4(GetPosition(), 1.0);
	fragColor = color;
}