#version 150

in vec3 inPosition;              

in vec3 inNormal;                out vec3 myNormal;
in vec2 inTexCoord;              out vec2 texCoord;

out vec3 fragPos;
out vec3 transformedNormal;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
	myNormal = inNormal;
	texCoord = inTexCoord;
  fragPos = vec3(mdlMatrix * vec4(inPosition, 1.0));

	mat3 normalMatrix1 = mat3(mdlMatrix);
	transformedNormal = normalMatrix1 * inNormal;

	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
}
