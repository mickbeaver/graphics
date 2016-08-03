VS_IN vec4 a_position;
VS_IN vec4 a_color;
VS_OUT vec4 v_theColor;

uniform mat4 u_cameraToClipMatrix;
uniform mat4 u_modelToCameraMatrix;

void
main()
{
    vec4 cameraPos = u_modelToCameraMatrix * a_position;
    gl_Position = u_cameraToClipMatrix * cameraPos;
    v_theColor = a_color;
}
