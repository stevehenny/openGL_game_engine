
#version 460 core 

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


// out to fragment
layout(location = 0) out vec3 Normal;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec2 TexCoord;


struct GravityObject {
  vec3 position;
  float mass;
};


layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;
layout(location = 13) uniform int numObjects;

layout(std430, binding = 0) buffer GravityBuffer{
  GravityObject objs[];
};

// layout(location = 20) uniform vec3 objectPos;
// layout(location = 21) uniform float mass;

// // tweakable parameters (you can expose these as uniforms if you like)
const float EPS = 0.5;        // avoids singularity near dist=0
const float EXPONENT = 2.0;
const float MASS_SCALE = 5.0e-11;
const float MAX_DISP = 1.0;   // maximum displacement magnitude (positive value)
const float EFFECT_RADIUS = 6.0; // beyond this distance the effect fades out
//

float calculateDisp(vec3 objPos, float mass){

    vec3 toObj = aPos - objPos;
    float dist = length(toObj);

    // compute base displacement (use scaled mass and exponent)
    // negative because we want a well (downwards)
    float scaledMass = mass * MASS_SCALE;
    float rawDisp = - scaledMass / pow(dist + EPS, EXPONENT);

    // clamp max displacement so it doesn't blow up
    float disp = rawDisp;

    // optionally fade out with a smoothstep near EFFECT_RADIUS
    float fade = 1.0 - smoothstep(EFFECT_RADIUS * 0.6, EFFECT_RADIUS, dist);
    disp *= fade;
    return disp;
}

void main() {
    // base position in model space
    vec3 pos = aPos;
    // vec3 displacement = vec3(0.0);

    for(int i = 0; i < numObjects; ++i){
        pos.y += calculateDisp(objs[i].position, objs[i].mass);
    }

    // pos += displacement * 0.01;// scale factor
    gl_Position = projection * view * model * vec4(pos, 1.0);
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord * 10.0;

}
