#version 150

in vec3 in_Position;
in vec3 in_Normal;
out vec3 myNormal;
uniform mat4 myMatrix;

void main(void)
{
  myNormal = in_Normal;
	gl_Position = vec4(in_Position, 1.0) * myMatrix;
}
