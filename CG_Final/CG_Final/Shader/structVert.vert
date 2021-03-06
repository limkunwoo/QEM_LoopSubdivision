// minimal vertex shader
// www.lighthouse3d.com

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

varying vec3 normal;
varying vec3 position;

void main()
{
	gl_Position = projection * view * model * gl_Vertex;
	gl_FrontColor = gl_Color;
	normal = gl_Normal;
	position = gl_Vertex;
}