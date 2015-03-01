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
out vec4 my_color;
/////////////////////////////
uniform vec3 lightPosition;
vec4 Lighting(vec3 normalInCam, vec4 vertsInCam, vec4 lightPosInCam)
{
    vec4 lightAmbient = vec4(1.0,1.0,1.0,1.0);
    vec4 materialAmbient = vec4(0.5,0.5,0.5,1.0);
    vec4 ambient = lightAmbient * materialAmbient;
    /////////////////////////////////////////////////
    vec4 lightDiffuse = vec4(1.0,1.0,1.0,1.0);
    vec4 materialDiffuse = vec4(1.0,1.0,1.0,1.0);
    vec3 diffuseLight = normalize(lightPosInCam.xyz - vertsInCam.xyz); // diffuse ray vertex to light in cam
    float duffuseFactor = max(dot(diffuseLight, normalInCam), 0.0);
    vec4 diffuse = duffuseFactor * lightDiffuse * materialDiffuse;
    /////////////////////////////////////////////////
	vec4 lightSpecular = vec4(1.0,1.0,1.0,1.0);
    vec4 materialSpecular = vec4(0.8,0.4,0.8,1.0);
	float material_shininess = 80.0;
	vec3 specularLight = vec3(vec3(0.0,0.0,0.0) - vertsInCam.xyz); // specular ray vertex to eye in cam
	vec3 reflectiveLight = reflect(-diffuseLight, normalInCam);  //  the reflective ray from (light to vertex == the negtive of "diffuse ray vertex to light in cam")
	float specularFactor = pow(max(dot(specularLight, reflectiveLight), 0.0), material_shininess);
	vec4 specular = specularFactor * materialSpecular * lightSpecular;
	if(dot(normalInCam, specularLight) < 0.0f)
	{
		specular = vec4(0.0,0.0,0.0,1.0);
	}

	return clamp(ambient + diffuse + specular, 0.0, 1.0);
}



void main()
{
    my_uv = uv;
    vec3 normInCam = (view * transpose(inverse(model)) * vec4(normal.xyz, 0.0)).xyz;
    vec4 vertsInCam = view * model * position;
    vec4 lightInCam = view * model * vec4(lightPosition.xyz, 1.0);
	my_color = Lighting(normInCam, vertsInCam, lightInCam);
    gl_Position = projection * view  * model *position;
}




