#version 330 core

out vec4 fragColor_out;
in vec2 my_uv;
in vec4 my_color; //TODO: from vetex shader
//////////////////////////
uniform sampler2D alpha_texture;
uniform sampler2D grass_texture;
uniform sampler2D dirt_texture;
uniform sampler2D rock_texture;
uniform sampler2D snow_texture;
///////////////////////////


void main()
{
	vec2 uv = 10 * my_uv;
	float r = texture(alpha_texture, my_uv).r;

	vec4 grass = texture(grass_texture, uv);
    vec4 dirt = texture(dirt_texture, uv);
	vec4 rock = texture(rock_texture, uv);
	vec4 snow = texture(snow_texture, uv);

	vec4 mainTex = mix(rock, snow, smoothstep(50, 100, r*100));
	vec4 middle = mix(dirt, mainTex, smoothstep(30, 49, r*100));
	vec4 finalTex = mix(grass, middle, smoothstep(0, 30, r*100));


	fragColor_out = finalTex * my_color;
	fragColor_out.a = 1.0f;
}


