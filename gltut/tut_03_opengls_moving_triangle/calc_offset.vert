VS_IN vec4 aPosition;
uniform float uLoopDuration;
uniform float uTime;

void
main()
{
    float timeScale = 3.14159 * 2.0 / uLoopDuration;
    float currentTime = mod(uTime, uLoopDuration);
    vec4 totalOffset = vec4(cos(currentTime * timeScale) * 0.5,
                            sin(currentTime * timeScale) * 0.5,
                            0.0,
                            0.0);
    gl_Position = aPosition + totalOffset;
}
