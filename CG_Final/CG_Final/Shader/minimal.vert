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
	normal = model * vec4(gl_Normal, 0);
	//normal = (transpose(inverse(mat3(model)))) * gl_Normal;
	position = model * gl_Vertex;
}