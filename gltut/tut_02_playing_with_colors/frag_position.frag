#ifdef GL_ES
precision mediump float;
#endif

uniform float uWindowHeight;

void
main()
{
    float lerpValue = gl_FragCoord.y / uWindowHeight;
    
    gl_FragColor = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), lerpValue);
}
