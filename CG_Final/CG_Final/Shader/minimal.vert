// minimal vertex shader
// www.lighthouse3d.com

uniform mat4 model;
uniform mat4 camera;

varying vec3 normal;

void main()
{
	gl_Position = camera * model * gl_Vertex;
	gl_FrontColor = gl_Color;
	normal = model * vec4(gl_Normal, 0);
}