#version 120
varying vec2 TexCoord;
void main(void)
{
	//gl_Position = ftransform();
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//TexCoord = vec2(1,0);
	TexCoord = gl_MultiTexCoord0.st;
}
