#version 150

out vec4 out_Color;
in vec3 myNormal;
in vec3 transformedNormal;
in vec3 fragPos;
in vec2 myTexCoord;
//in vec3 gouraud;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform vec3 viewpos;
uniform bool needTexture;
uniform bool isSkybox;
uniform float mixAlpha;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent[4];
uniform bool isDirectional[4];

vec3 createAmbient(vec3 color, float strength)
{
  return strength * color;
}

vec3 createDiffuse(vec3 color, vec3 pos, vec3 fragPos, vec3 normal, float strength)
{
  return max(dot(normal, normalize(pos - fragPos)), 0) * color * strength;
}

vec3 createDirectional(vec3 color, vec3 dir, vec3 normal, float strength)
{
  return max(dot(normal, normalize(dir)), 0) * color * strength;
}

vec3 createSpecular(vec3 color, vec3 pos, vec3 fragPos, vec3 normal, float expo, float strength)
{
  //reflectDir
  vec3 phongLightDir = normalize(pos - fragPos);
  vec3 reflectDir = -phongLightDir - 2.0f * dot(-phongLightDir, transformedNormal) * transformedNormal;

  //viewDir
  vec3 viewDir = normalize(viewpos - fragPos);

  //phone
  return pow(max(dot(viewDir, reflectDir), 0.0f), expo) * strength * color;
}

void main(void)
{
  if(isSkybox)
  {
    out_Color = texture(tex1, myTexCoord) + vec4(1,1,1,1) * 0.5f * mixAlpha;
  }
  else
  {
    vec3 light = vec3(0,0,0);

    for(int i = 0; i < 4; i++)
    {
      if(isDirectional[i])
      light += createDirectional(lightSourcesColorArr[i], lightSourcesDirPosArr[i], transformedNormal, 1.f);
      else
       //light += createSpecular(lightSourcesColorArr[i], lightSourcesDirPosArr[i], fragPos, transformedNormal, specularExponent[i], 1.f);
       light += createDiffuse(lightSourcesColorArr[i], lightSourcesDirPosArr[i], fragPos, transformedNormal, 1.f);
    }

    if(needTexture)
      out_Color = mix(texture(tex2, myTexCoord), texture(tex3, myTexCoord), mixAlpha);
    else
      out_Color = vec4(light, 1.0f);
  }
}
