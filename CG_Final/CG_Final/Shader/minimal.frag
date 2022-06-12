// minimal fragment shader
// www.lighthouse3d.com

varying vec3 normal;
void main()
{
	float intensity = saturate(dot(vec3(0, 1, 0), normal));
	if(intensity >= 0)
	{
		gl_FragColor = intensity * gl_Color;
	}
	else
	{
		gl_FragColor = 0.4;
	}
	gl_FragColor = vec4(normal, 1);
}