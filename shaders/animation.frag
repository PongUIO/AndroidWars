uniform vec2 TexCoord;
uniform sampler2D tex;

void main(void)
{
    gl_FragColor = texture2D(tex, TexCoord);
    //gl_FragColor = vec4(1,0.5,1,1);
}
