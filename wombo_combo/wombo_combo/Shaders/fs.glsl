#version 330 core

out vec4 fragColor_out;
in vec2 my_uv;
in vec3 my_color;
//////////////////////////
uniform sampler2D alpha_texture;
uniform sampler2D grass_texture;
uniform sampler2D dirt_texture;
uniform sampler2D rock_texture;
uniform sampler2D snow_texture;
///////////////////////////

void main()
{
    fragColor_out = texture(grass_texture, my_uv*50);
}

