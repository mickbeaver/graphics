VS_IN vec4 aPosition;
uniform vec2 uOffset;

void
main()
{
    vec4 totalOffset = vec4(uOffset.x, uOffset.y, 0.0, 0.0);
    gl_Position = aPosition + totalOffset;
}
