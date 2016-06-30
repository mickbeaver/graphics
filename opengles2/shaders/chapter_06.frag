#ifdef GL_ES
precision mediump float;
#endif

PS_IN vec4 v_color;

void
main()
{
    gl_FragColor = v_color;
}
