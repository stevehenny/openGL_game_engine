#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec4 Normal;
layout(location = 1) out vec4 FragPos;
layout(location = 2) out vec2 TexCoord;
layout(location = 3) flat out int sphereIndex;

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

layout(location = 10) uniform int numSpheres;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;

void main()
{
  int instance = gl_InstanceID;

  vec3 worldPos = spheres[instance].position.xyz + aPos;

  FragPos = vec4(worldPos, 1.0f);
  Normal = vec4(aNormal, 1.0f);
  TexCoord = aTexCoord;
  sphereIndex = instance;

  gl_Position = projection * view * vec4(worldPos, 1.0);
}
