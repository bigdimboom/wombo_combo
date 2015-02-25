#version 330 core

out vec4 fragColor_out;

in vec2 my_uv;
in vec3 my_normal;

uniform sampler2D grass_texture;
uniform sampler2D dirt_texture;
uniform sampler2D rock_texture;
uniform sampler2D snow_texture;


vec3 blend(vec4 texture1, float a1, vec4 texture2, float a2)
{
    float depth = 0.2;
    float ma = max(texture1.a + a1, texture2.a + a2) - depth;

    float b1 = max(texture1.a + a1 - ma, 0);
    float b2 = max(texture2.a + a2 - ma, 0);

    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);
}


void main()
{
	//vec2 reg1 = vec2(my_uv.s / 4, my_uv.t / 4);
	//vec2 reg2 = vec2(my_uv.s / 6, my_uv.t / 6);
	vec3 tex1 = blend(texture(grass_texture, my_uv), 1, texture(dirt_texture, my_uv), 0);
	//vec3 tex2 = blend(texture(dirt_texture, my_uv), 100, texture(rock_texture, my_uv), 100);
	//vec3 tex3 = blend(texture(rock_texture, my_uv), 100, texture(snow_texture, my_uv), 0.01);
	fragColor_out = vec4((tex1).xyz, 1.0);
}

