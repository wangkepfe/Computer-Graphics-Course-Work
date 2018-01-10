#version 150

out vec4 out_Color;
in vec3 myNormal;
in vec2 myTexCoord;

void main(void)
{
	out_Color = vec4(myTexCoord[0],myTexCoord[1],0.0f,1.0f);
}
