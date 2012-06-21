uniform sampler2D qt_Texture0;
varying highp vec4 qt_TexCoord0;
varying vec2 tex;

void main(void)
{
    gl_FragColor = min(texture2D(qt_Texture0, tex), gl_Color);
    gl_FragColor[3] = gl_Color[3];
}
