#version 460 core

// vertex array layout

struct Particle {
  vec4 position;
  vec4 velocity;
  vec2 amplitude;
  float probability;
  bool collapsed;
};

layout(std430, binding = 0) buffer ParticleBuffer {
  Particle particles[];
};

// uniform layout
layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;
layout(location = 13) uniform int numObjects;

void main() {}
