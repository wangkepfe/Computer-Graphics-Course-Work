#version 150

out vec4 out_Color;
in vec3 myNormal;
in vec3 transformedNormal;
in vec3 fragPos;
in vec2 myTexCoord;
in vec3 gouraud;
uniform sampler2D texUnit;
uniform vec3 viewpos;

void main(void)
{
  //ambient
  float ambientStrength = 0.05f;
  vec3 ambientLightColor = vec3(1,1,1);
  vec3 ambient = ambientStrength * ambientLightColor;

  //diffuse
  vec3 diffuseLightColor = vec3(0.4f,0.4f,0.4f);
  vec3 diffuseLightPos = vec3(2,1,1);
  vec3 diffuseLightDir = normalize(diffuseLightPos - fragPos);
  vec3 diffuse = max(dot(transformedNormal, diffuseLightDir), 0) * diffuseLightColor;

  //phong
  float phongStrengh = 5.0f;
  vec3 phongColor = vec3(0.5f,0.5f,0.5f);
  vec3 phongLightPos = vec3(2,1,1);
  vec3 phongLightDir = normalize(phongLightPos - fragPos);
  vec3 viewDir = normalize(viewpos - fragPos);
  vec3 reflectDir = -phongLightDir - 2.0f * dot(-phongLightDir, transformedNormal) * transformedNormal;
  vec3 phong = pow(max(dot(viewDir, reflectDir), 0.0f), 16) * phongStrengh * phongColor;

  //texture
  vec4 textureColor = texture(texUnit, myTexCoord);

  //
  out_Color = vec4(ambient + diffuse + phong + gouraud, 1.0f) + textureColor;
}
