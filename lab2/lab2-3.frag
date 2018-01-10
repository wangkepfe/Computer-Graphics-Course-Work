#version 150

out vec4 out_Color;
in vec3 myNormal;
in vec2 myTexCoord;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, myTexCoord);
}
