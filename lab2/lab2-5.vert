#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
out vec2 myTexCoord;
out vec3 myNormal;
out vec3 transformedNormal;
out vec3 fragPos;
uniform mat4 myMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;


void main(void)
{
  myNormal = in_Normal;
  myTexCoord = inTexCoord;

  mat3 normalMatrix1 = mat3(myMatrix);
  transformedNormal = normalMatrix1 * in_Normal;

  fragPos = vec3(mdlMatrix * myMatrix * vec4(in_Position, 1.0));

	gl_Position = projectionMatrix * cameraMatrix * mdlMatrix * myMatrix * vec4(in_Position, 1.0);
}
