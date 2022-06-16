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

	float rim = 1 - max(0, dot(normal, toCam));
	rim = pow(rim, 4);

	float diffuse = max(dot(light, normal), 0);
	if(diffuse >= 0)
	{
		gl_FragColor = (diffuse * gl_Color + rim) + 0.1;
		gl_FragColor *= gl_Color;	
	}
	//gl_FragColor = vec4(vec3(1,0,0) * rim, 1);
}