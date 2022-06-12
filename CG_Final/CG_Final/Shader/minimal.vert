// minimal vertex shader
// www.lighthouse3d.com

uniform mat4 model;
uniform mat4 camera;
void main()
{
	gl_Position = camera * model * gl_Vertex;
	gl_FrontColor = gl_Color;
}