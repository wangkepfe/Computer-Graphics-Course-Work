#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
out vec2 myTexCoord;
out vec3 myNormal;
uniform mat4 myMatrix;

void main(void)
{
  myNormal = in_Normal;
  myTexCoord = inTexCoord;
	gl_Position = vec4(in_Position, 1.0) * myMatrix;
}
