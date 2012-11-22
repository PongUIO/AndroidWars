#version 330
uniform vec4 color;
uniform vec4 edgeColor;
uniform sampler2D qt_Texture0;

layout(location=0) out vec4 colorOut;

in vec3 normal;

void main(void)
{
	colorOut = vec4(normal, 1);
}
