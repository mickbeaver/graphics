uniform mat4 uRotation;
VS_IN vec4 aPosition;
VS_IN vec4 aColor;
VS_OUT vec4 vColor;

void
main()
{
	gl_Position = uRotation * aPosition;
	vColor = aColor;
}

