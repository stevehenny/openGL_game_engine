#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// Outputs to fragment shader
layout(location = 0) out vec3 Normal;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec2 TexCoord;

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

// Uniforms
layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;
layout(location = 13) uniform int numObjects;

// SSBO with gravity spheres
layout(std430, binding = 0) buffer GravityBuffer {
  Sphere objs[];
};

// Gravity parameters
const float EPS = 0.5;
const float EXPONENT = 2.0;
const float MASS_SCALE = 6.674e-11;
const float EFFECT_RADIUS = 6.0;

float calculateDisp(vec3 worldPos, vec3 objPos, float mass) {
  vec3 toObj = worldPos - objPos;
  float dist = length(toObj);

  float scaledMass = mass * MASS_SCALE;
  float rawDisp = -scaledMass / pow(dist + EPS, EXPONENT);

  return rawDisp;
  // float fade = 1.0 - smoothstep(EFFECT_RADIUS * 0.6, EFFECT_RADIUS, dist);
  // return rawDisp * fade;
}

void main() {
  // Transform vertex to world space first
  vec3 worldPos = vec3(model * vec4(aPos, 1.0));

  // Apply gravity displacement from all spheres
  for (int i = 0; i < numObjects; ++i) {
    worldPos.y += calculateDisp(worldPos, objs[i].position.xyz, objs[i].mass);
  }

  // Output final clip-space position
  gl_Position = projection * view * vec4(worldPos, 1.0);

  // Pass world-space position and normal to fragment shader
  FragPos = worldPos;
  Normal = mat3(transpose(inverse(model))) * aNormal;

  // Scale UVs as desired
  TexCoord = aTexCoord * 10.0;
}
