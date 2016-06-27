#ifdef GL_ES
precision mediump float;
#endif

VS_OUT vec4 v_color;

void
main()
{
    gl_FragColor = v_color;
}
