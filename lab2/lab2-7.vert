#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;
out vec2 myTexCoord;
out vec3 myNormal;
out vec3 transformedNormal;
out vec3 fragPos;
out vec3 gouraud;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 cameraMatrix;
uniform vec3 viewpos;

void main(void)
{
  myNormal = in_Normal;
  myTexCoord = inTexCoord;

  mat3 normalMatrix1 = mat3(cameraMatrix * mdlMatrix);
  transformedNormal = normalMatrix1 * in_Normal;

  fragPos = vec3(mdlMatrix * vec4(in_Position, 1.0));

  //gouraud
  float gouraudStrengh = 1.0f;
  vec3 gouraudColor = vec3(0.5f,0.5f,0.5f);
  vec3 gouraudLightPos = vec3(2,1,1);
  vec3 gouraudLightDir = normalize(gouraudLightPos - fragPos);
  vec3 viewDir = normalize(viewpos - fragPos);
  vec3 reflectDir = -gouraudLightDir - 2.0f * dot(-gouraudLightDir, transformedNormal) * transformedNormal;
  gouraud = pow(max(dot(viewDir, reflectDir), 0.0f), 16) * gouraudStrengh * gouraudColor;

	gl_Position = projectionMatrix * cameraMatrix * mdlMatrix * vec4(in_Position, 1.0);
}
