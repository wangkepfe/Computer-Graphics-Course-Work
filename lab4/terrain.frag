#version 150

out vec4 outColor;

in vec2 texCoord;

in vec3 fragPos;

in vec3 myNormal;
in vec3 transformedNormal;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform vec3 viewpos;
uniform vec3 lightpos;

uniform float heightRamp[4];
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

  //phong
  return pow(max(dot(viewDir, reflectDir), 0.0f), expo) * strength * color;
}

void main(void)
{
	vec3 light = vec3(0,0,0);
	light += createDiffuse(vec3(1,1,1),lightpos,fragPos,myNormal,0.2f);
	light += createSpecular(vec3(1,1,1),lightpos,fragPos,myNormal,8,0.2f);
  outColor = vec4(light, 1.0f);

  //nulti texture terrain
  if(fragPos[1] <= heightRamp[0])
  {
    outColor += texture(tex0, texCoord);
  }
  else if(heightRamp[0] <= fragPos[1] && fragPos[1] < heightRamp[1])
  {
    outColor += mix(texture(tex0, texCoord), texture(tex1, texCoord), (fragPos[1] - heightRamp[0]) / (heightRamp[1] - heightRamp[0]));
  }
  else if(heightRamp[1] <= fragPos[1] && fragPos[1] < heightRamp[2])
  {
    outColor += texture(tex1, texCoord);
  }
  else if(heightRamp[2] <= fragPos[1] && fragPos[1] < heightRamp[3])
  {
    outColor += mix(texture(tex1, texCoord), texture(tex2, texCoord), (fragPos[1] - heightRamp[2]) / (heightRamp[3] - heightRamp[2]));
  }
  else if(heightRamp[3] <= fragPos[1])
  {
    outColor += texture(tex2, texCoord);
  }
}
