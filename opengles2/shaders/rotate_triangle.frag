#ifdef GL_ES
precision mediump float;
#endif

VS_OUT vec4 vColor;

void
main()
{
    gl_FragColor = vColor;
}
