VS_IN vec4 a_position;
VS_IN vec4 a_color;
VS_OUT vec4 v_color;

void
main()
{
	gl_Position = a_position;
	v_color = a_color;
}
