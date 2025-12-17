#version 460 core

layout(location = 0) in vec4 Normal;
layout(location = 1) in vec4 FragPos;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) flat in int sphereIndex;

layout(location = 0) out vec4 FragColor;

struct Sphere {
  vec4 objectColor;
  vec4 position;
  vec4 force;
  vec4 velocity;
  vec4 Norm;
  vec4 lightColor;
  float specStrength;
  float ambStrength;
  float radius;
  float mass;
  int textureInd;
  bool isLightSource;
};

layout(std430, binding = 3) buffer SphereBuffer {
  Sphere spheres[];
};

// const int NUM_TEX = 128;

// layout(std430, binding = 4) buffer LightBuffer {
//   LightSource sources[];
// };

layout(location = 10) uniform uint numSpheres;
layout(location = 19) uniform vec4 viewPos;
layout(location = 20) uniform sampler2DArray textures;
// layout(location = 20) uniform sampler2D textures[NUM_TEX];

vec4 computeLighting(vec4 lightPos, vec4 lightColor, vec4 baseColor)
{
  float ambientStrength = 0.1;
  vec4 ambient = ambientStrength * lightColor;
  vec4 lightDir = normalize(lightPos - FragPos);
  vec4 norm = normalize(Normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec4 diffuse = diff * lightColor;
  float specularStrength = 0.5;
  vec4 viewDir = normalize(viewPos - FragPos);
  vec4 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec4 specular = specularStrength * spec * lightColor;
  return (ambient + diffuse + specular) * baseColor;
}

void main()
{
  vec4 texColor = texture(textures, vec3(TexCoord, spheres[sphereIndex].textureInd));
  vec4 baseColor = texColor * spheres[sphereIndex].objectColor;
  vec4 result = baseColor;

  for (uint i = 0; i < numSpheres; ++i) {
    if (spheres[i].isLightSource)
      // result += computeLighting(spheres[i].position, spheres[i].lightColor, baseColor);
      result = vec4(0.0f);
  }
  FragColor = result;
}
