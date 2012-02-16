#version	120

uniform sampler2D	backside;
uniform sampler2D	frontside;
uniform sampler1D	transferfunc;
uniform sampler3D	volume;

uniform float		width;
uniform float		height;
uniform float		depth;

uniform bool		transparent;

uniform vec2		xculling;
uniform vec2		yculling;
uniform vec2		zculling;

uniform vec3		eyeposition;
uniform vec3		lightposition;


float				stepSize = 1/max(width,max(height,depth));
float				delta = stepSize;
int					iterations = int(sqrt(pow(depth,2)+pow(width,2)+pow(height,2)));

	
vec3	shading( vec3 normal , vec3 light , vec3 view )
{
	vec3	Ka = vec3(0.1,0.1,0.1);
	vec3	Kd = vec3(0.7,0.7,0.7);
	vec3	Ks = vec3(0.2,0.2,0.2);
	float	shininess = 100.0;
	
	vec3	lightColour = vec3(1.0,1.0,1.0);
	vec3	ambientLight = vec3(0.0,0.0,0.0);


	vec3	half = normalize(light+view);
	
	float	diffuseLight = max(dot(light,normal),0);
	float	specularLight = pow(max(dot(half,normal),0),shininess);
	if ( diffuseLight <= 0 )	specularLight = 0;


	vec3	ambient = Ka*ambientLight;
	vec3	diffuse = Kd*lightColour*diffuseLight;
	vec3	specular = Ks*lightColour*specularLight;

	return ambient+diffuse+specular;
};


void main()
{
	vec4	back = texture2D(backside,gl_TexCoord[0].st);
	vec4	front = texture2D(frontside,gl_TexCoord[0].st);

	vec3	direction = front.xyz - back.xyz;
	vec3	directionNormal = normalize(direction);
	float	directionLength = length(direction);

	vec3	move = directionNormal * stepSize;
	float	moveLength = length(move);
	
	vec3	ray = back.xyz;
	vec4	returnValue = vec4(0,0,0,0);
	float	alphaSum = 0.0;
	float	lengthSum = 0.0;
	vec4	colorSample;
	float	alphaSample;
	vec4	transferSample;



	for ( int i = 0;  i < iterations;  i++ )
	{
		if ( ray.x >= xculling.x  &&  ray.x <= xculling.y  &&  ray.y >= yculling.x  &&  ray.y <= yculling.y  &&  ray.z >= zculling.x  &&  ray.z <= zculling.y )
		{
			colorSample = texture3D(volume,ray);
			transferSample = texture1D(transferfunc,colorSample.r);


			if ( transferSample.a > 0.0 )
			{
				if ( !transparent )
					alphaSample = transferSample.a;
				else
					alphaSample = transferSample.a*0.005;


				if ( !transparent )
				{
					vec3	sample , sample2 , alpha , alpha2;



					sample.x = texture3D(volume,ray-vec3(delta,0,0)).x;
					sample.y = texture3D(volume,ray-vec3(0,delta,0)).x;
					sample.z = texture3D(volume,ray-vec3(0,0,delta)).x;

					sample2.x = texture3D(volume,ray+vec3(delta,0,0)).x;
					sample2.y = texture3D(volume,ray+vec3(0,delta,0)).x;
					sample2.z = texture3D(volume,ray+vec3(0,0,delta)).x;


					vec3	normal = normalize(sample2-sample);
					vec3	light = normalize(lightposition - ray);
					vec3	view = normalize(eyeposition - ray);


					transferSample.rgb += shading(normal,light,view);
				}


				returnValue = (1-alphaSample)*returnValue + transferSample*alphaSample;
				alphaSum = (1-alphaSample)*alphaSum + transferSample.a*alphaSample;
			}
		}
		
		ray += move;
		lengthSum += moveLength;
		if ( lengthSum >= directionLength  ||  alphaSum > 1.0 )	break;
	}


	gl_FragColor = returnValue;
}