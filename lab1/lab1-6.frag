#version 150

out vec4 out_Color;
in vec3 myNormal;

void main(void)
{
	out_Color = vec4(myNormal[1] * myNormal[1],myNormal[1],myNormal[1],1.0f);
}
