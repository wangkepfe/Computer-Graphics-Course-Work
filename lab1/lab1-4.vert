#version 150

in vec3 in_Position;
in vec3 in_Color;
out vec3 ourColor;
uniform mat4 myMatrix;

void main(void)
{
  ourColor = in_Color;
	gl_Position = vec4(in_Position, 1.0) * myMatrix;
}
