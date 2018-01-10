#version 150

out vec4 out_Color;
in vec3 myNormal;
in vec3 transformedNormal;
in vec3 fragPos;
in vec2 myTexCoord;
uniform sampler2D texUnit;

void main(void)
{
  //ambient
  float ambientStrength = 0.0f;
  vec3 ambientLightColor = vec3(1,1,1);
  vec3 ambient = ambientStrength * ambientLightColor;

  //diffuse
  vec3 diffuseLightColor = vec3(0.5f,0.5f,0.5f);
  vec3 diffuseLightPos = vec3(4,5,4);
  vec3 diffuseLightDir = normalize(diffuseLightPos - fragPos);
  vec3 diffuse = max(dot(transformedNormal, diffuseLightDir), 0) * diffuseLightColor;

  //texture
  vec4 textureColor = texture(texUnit, myTexCoord);

  //
  out_Color = vec4(ambient + diffuse, 1.0f) + textureColor;
}
