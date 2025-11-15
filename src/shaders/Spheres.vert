#version 460 core


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 Normal;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec2 TexCoord;
layout(location = 3) flat out int sphereIndex;

struct Sphere {
  vec4 objectColor;
  vec4 position;
  vec4 force;
  vec4 velocity;
  float radius;
  float mass;
  int textureInd;
  int pad;
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

    // Use position from SBBO
    vec3 worldPos = aPos + spheres[instance].position.xyz;

    FragPos = worldPos;
    Normal = aNormal; // No rotation applied
    TexCoord = aTexCoord;
    sphereIndex = instance;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}
