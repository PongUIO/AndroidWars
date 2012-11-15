#version 330
uniform vec4 color;
uniform sampler2D qt_Texture0;

in vec2 texOut;

layout(location=0) out vec4 colorOut;

void main(void)
{
	//colorOut = vec4(texOut, 0, 1);//texture2D(qt_Texture0, texOut);//min(texture2D(qt_Texture0, texOut), color);
	colorOut = color;
	colorOut[3] = color[3];
}
