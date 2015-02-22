#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec3 color;
out vec2 uv;

out vec3 fN; 
out vec3 fE;
out vec3 fL;

out int textureType;

void main()
{
   gl_Position = projection*view*position;
  // color = normal;
   uv = texCoord;

   
   vec4 lightPosition = vec4(1.8, 100, 1, 0);

   //vertex, normal and light in camera coordinates
   vec3 pos = (view * position).xyz; // vertex in camera
   vec3 lightCC = (view*lightPosition).xyz; // light in camera
   fN = normalize((view*vec4(normal,0.0)).xyz); // normal in camera
    //ray from vertex towards camera
   fE =normalize(-pos);
    //ray from vertex towards light
   fL = normalize(lightCC-pos);

   if(position.y <= 0){ textureType = 1;}
   else{ textureType = 2;}
}