#ifdef GL_ES
precision mediump float;
#endif

PS_IN vec4 i_theColor;

void
main()
{
    gl_FragColor = i_theColor;
}
