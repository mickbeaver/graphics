#ifdef GL_ES
precision mediump float;
#endif

varying vec4 vColor;

void
main()
{
    float lerpValue = gl_FragCoord.y / 500.0;
    
    gl_FragColor = vColor * lerpValue; //mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), lerpValue);
}
