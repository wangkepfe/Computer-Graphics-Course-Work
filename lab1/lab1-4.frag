#version 150

in vec3 ourColor;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(ourColor, 1.0f);
}
