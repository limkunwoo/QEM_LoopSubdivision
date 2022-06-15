// minimal fragment shader
// www.lighthouse3d.com

uniform vec3 cameraPos;

varying vec3 normal;
varying vec3 position;

void main()
{
	normal = normalize(normal);
	vec3 toCam = normalize(cameraPos - position);
	vec3 light = normalize(vec3(1,1,0));

	float rim = max(0, dot(normal, toCam));
	rim = pow(rim, 2);

	float diffuse = max(dot(light, normal), 0);
	if(diffuse >= 0)
	{
		gl_FragColor = (diffuse * gl_Color + rim) + 0.13;
		gl_FragColor *= gl_Color;
	}
	else
	{
		gl_FragColor = 0.4;
	}
	//gl_FragColor = vec4(vec3(1,0,0) * rim, 1);
}