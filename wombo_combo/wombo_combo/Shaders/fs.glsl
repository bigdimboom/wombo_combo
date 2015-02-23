#version 330 core

//in vec3 color;
in vec2 uv;

in vec3 fN;
in vec3 fL;
in vec3 fE;

flat in int textureType;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
	//gl_FragColor = vec4(color, 1.0);
	//gl_FragColor = texture(myTexture, uv) * vec4(color, 1.0);
  
  vec4 light_ambient = vec4(0.3, 0.3, 0.3, 1);
  vec4 light_diffuse = vec4(1.0, 1.0, 1.0, 1);
  vec4 light_specular = vec4(1.0, 1.0, 1.0, 1);

  vec4 material_ambient = vec4(1.0, 1.0, 1.0, 1.0);
  vec4 material_diffuse = vec4(1.0, 0.8, 0.0, 1.0);
  vec4 material_specular = vec4(0.8, 0.8, 0.8, 1.0);
  float material_shininess = 5.0;

  vec4 AmbientProduct = light_ambient * material_ambient;
  vec4 DiffuseProduct = light_diffuse * material_diffuse;
  vec4 SpecularProduct = light_specular * material_specular;
  
  vec3 L = normalize(fL);
  vec3 E = normalize(fE);
  vec3 N = normalize(fN);
  //vec3 H = normalize( L + E );
  
  vec4 ambient = AmbientProduct; // ambient
  
  float Kd = max(dot(L, N), 0.0);
  vec4 diffuse = Kd*DiffuseProduct; // diffuse

  vec3 Reflective = reflect(-fE, fN);
  float Ks = pow(max(dot(E, Reflective), 0.0), material_shininess);
  vec4 specular = Ks*SpecularProduct; // specular
  if( dot(L, N) < 0.0 ) {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
  }

  vec4 lighting_color = diffuse + specular + ambient;

  if (textureType == 1)
		gl_FragColor = mix(texture(myTexture1, uv), texture(myTexture2, uv), 0.1) * lighting_color ;

  if (textureType == 2)
		gl_FragColor = mix(texture(myTexture1, uv), texture(myTexture2, uv), 0.8) * lighting_color;

  //gl_FragColor.a = 1.0;
}