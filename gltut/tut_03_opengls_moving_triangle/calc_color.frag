#ifdef GL_ES
precision mediump float;
#endif

uniform float uTime;
uniform float uFragLoopDuration;

const vec4 firstColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 secondColor = vec4(0.0, 0.0, 0.0, 1.0);

void
main()
{
    float currentTime = mod(uTime, uFragLoopDuration);
    float currentLerp = sin((currentTime / uFragLoopDuration) * 3.14159);
    
    gl_FragColor = mix(firstColor, secondColor, currentLerp);
}
