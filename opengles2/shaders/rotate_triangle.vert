#version 100

uniform mat4 uRotation;
attribute vec4 aPosition;
attribute vec4 aColor;
varying vec4 vColor;

void
main()
{
	gl_Position = uRotation * aPosition;
	vColor = aColor;
}

