#version 400
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vVelocity;
layout(location = 2) in vec4 vColor;
layout(location = 3) in float fLife;
layout(location = 4) in float fSize;
layout(location = 5) in int iType;

out VSOutData
{
	vec3 vPosition;
	vec3 vVelocity;
	vec4 vColor;
	float fLife;
	float fSize;
	in iType;
}vOut;

void Assign()
{
	vOut.vPosition = vPosition;
	vOut.vVelocity = vVelocity;
	vOut.vColor = vColor;
	vOut.fLife = fLife;
	vOut.fSize = fSize;
	vOut.iType = iType;
}

void main()
{
	Assign();
}