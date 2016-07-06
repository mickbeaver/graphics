VS_IN vec4 a_position;
VS_IN vec4 a_color;

VS_OUT vec4 v_theColor;

uniform vec3 u_offset;
uniform mat4 u_perspectiveMatrix;

void
main()
{
    vec4 cameraPos = a_position + vec4(u_offset, 0.0);

    gl_Position = u_perspectiveMatrix * cameraPos;
    v_theColor = a_color;
}
