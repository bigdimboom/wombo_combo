#version 400

layout(points) in; 
layout(points) out; 
layout(max_vertices = 40) out;

// All that we get from vertex shader

out VSOutData
{
	vec3 vPosition;
	vec3 vVelocity;
	vec4 vColor;
	float fLife;
	float fSize;
	in iType;
}gIn[];

out GSOutData
{
	vec3 vPostion;
	vec3 vVelocity;
	vec4 vColor;
	float fLife;
	float fSize;
	in iType;
}gsOut;

uniform ParticleGenerator
{
	vec3 vPosition;
	vec3 vGravityVector;
	vec3 vVelocityMin;
	vec3 vVelocityRange; 

	vec4 vColor; 
	float fSize;  

	float fLifeMin, fLifeRange;
	float fTimePassed;

	vec3 vRandomSeed;
	
	int iNumToGenerate;
}p;

vec3 vLocalSeed; 

// This function returns random number from zero to one
float randZeroOne()
{
	uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + 
							 vLocalSeed.x * 2531011.0 + 
							 vLocalSeed.z * 141251.0);
	n = n * (n * n * 15731u + 789221u);
	n = (n >> 9u) | 0x3F800000u;

	float fRes =  2.0 - uintBitsToFloat(n);
	p.vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, 
						vLocalSeed.y * fRes + 415161.0 * fRes, 
						vLocalSeed.z + 324154.0 * fRes);

	return fRes;
}

void main()
{
	vLocalSeed = p.vRandomSeed;
	gsOut.vPosition = gIn[0].vPosition;
	gsOut.vVelocity = gIn[0].vVelocity;

	if(gIn[0].iType != 0)
	{
		gsOut.vPosition += gsOut.vVelocity * p.fTimePassed;
		gsOut.vVelocity += p.vGravityVector * p.fTimePassed;
	}

	gsOut.vColor = gIn[0].vColor;
	gsOut.fLife = gIn[0].fLife - p.fTimePassed;
	gsOut.fSize = gIn[0].fSize;
	gsOut.iType = gIn[0].iType;

	if( iType == 0 )
	{
		EmitVertex();
		EndPrimitive();

		for(int i = 0; i < p.iNumToGenerate; ++i)
		{
			gsOut.vPosition = p.vPosition;
			gsOut.vVelocity = p.vVelocityMin + vec3(p.vVelocityRange.x * randZeroOne(),
											   p.vVelocityRange.y * randZeroOne(),
											   p.vVelocityRange.z * randZeroOne());
			gsOut.vColor = p.vColor;
			gsOut.fLife = p.fLifeMin + p.fLifeRange * randZeroOne();
			gsOut.fSize = p.fSize;
			gsOut.iType = 1;
			EmitVertex(); 
			EndPrimitive(); 
		}
	}
	else if(gsOut.fLife > 0.0f)
	{
		EmitVertex(); 
		EndPrimitive();  
	}

}
