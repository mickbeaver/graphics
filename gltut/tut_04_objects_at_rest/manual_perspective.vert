VS_IN vec4 a_position;
VS_IN vec4 a_color;
VS_OUT vec4 i_theColor;

uniform vec2 u_offset;
uniform float u_zNear;
uniform float u_zFar;
uniform float u_frustumScale;

void
main()
{
    vec4 cameraPos = a_position + vec4(u_offset.x, u_offset.y, 0.0, 0.0);
    vec4 clipPos;

    clipPos.xy = cameraPos.xy * u_frustumScale;

    clipPos.z = cameraPos.z * (u_zNear + u_zFar) / (u_zNear - u_zFar);
    clipPos.z += 2 * u_zNear * u_zFar / (u_zNear - u_zFar);

    clipPos.w = -cameraPos.z;

    gl_Position = clipPos;
    i_theColor = a_color;
}
