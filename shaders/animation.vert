#version 330

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

in vec3 vertex;
in vec3 inNormal;

out vec3 normal;

void main(void)
{
	//texOut = gl_MultiTexCoord0.xy;
	//gl_Position = vec4(vertex, 1.0);
	gl_Position = projection_matrix * modelview_matrix * vec4(vertex, 1.0);
	normal = inNormal;
}
