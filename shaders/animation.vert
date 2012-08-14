#version 120
varying highp vec4 qt_TexCoord0;
varying vec2 tex;

void main(void)
{
	gl_Position = ftransform();
	tex = gl_MultiTexCoord0.xy;
	gl_FrontColor = gl_Color;
        //gl_FrontSecondaryColor = gl_SecondaryColor;
}
