VS_IN vec4 a_position;
VS_IN vec4 a_color;

VS_OUT vec4 i_theColor;

uniform vec2 u_offset;

void
main()
{
    gl_Position = a_position + vec4(u_offset.x, u_offset.y, 0.0, 0.0);
    i_theColor = a_color;
}
