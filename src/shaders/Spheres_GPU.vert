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
};

layout(std430, binding = 3) buffer SphereBuffer {
    Sphere spheres[];
};

layout(location = 10) uniform int numSpheres;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;


const float G = 6.674e-11f;
const float dt = 0.000001f;

void main()
{
    int instance = gl_InstanceID;

    // if (instance >= numSpheres) return;

    // compute forces on spheres
    for(int i = 0; i < numSpheres; ++i){
      if(instance == i) continue;

      vec3 r_vec = vec3(spheres[instance].position - spheres[i].position);
      float r_mag = length(r_vec);
      if(r_mag == 0) continue;

      vec3 f_dir = r_vec / r_mag;
      float f_mag = G * spheres[instance].mass * spheres[i].mass / (r_mag * r_mag);
      vec3 f_vec = f_mag * f_dir;

      spheres[instance].force -= vec4(f_vec, 0.0f);
      spheres[i].force += vec4(f_vec, 0.0f);
    }

    // apply forces to position dependent on time change
    vec3 acceleration = vec3(spheres[instance].force )/ spheres[instance].mass;
    
    spheres[instance].velocity += vec4(acceleration * dt , 0.0);
    spheres[instance].position += vec4(vec3(spheres[instance].velocity) * dt, 0.0);
  
    
    

    // Use position from SBBO
    vec3 worldPos = aPos + spheres[instance].position.xyz;

    FragPos = worldPos;
    Normal = aNormal; // No rotation applied
    TexCoord = aTexCoord;
    sphereIndex = instance;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}
