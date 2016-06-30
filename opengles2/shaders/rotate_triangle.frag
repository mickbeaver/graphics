#ifdef GL_ES
precision mediump float;
#endif

PS_IN vec4 vColor;

void
main()
{
    gl_FragColor = vColor;
}
