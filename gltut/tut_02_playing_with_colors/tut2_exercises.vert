VS_IN vec4 aPosition;
VS_IN vec4 aColor;
VS_OUT vec4 vColor;

void
main()
{
    vColor = aColor;
    gl_Position = aPosition;
}
