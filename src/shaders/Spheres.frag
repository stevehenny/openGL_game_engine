#version 460 core

layout(location = 0) in vec3 Normal;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) flat in int sphereIndex;

layout(location = 0) out vec4 FragColor;

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

const int NUM_TEX = 128;

layout(location = 14) uniform sampler2D texture1;
layout(location = 15) uniform vec3 lightColor;
layout(location = 16) uniform vec3 lightPos;
layout(location = 17) uniform vec3 lightColor2;
layout(location = 18) uniform vec3 lightPos2;
layout(location = 19) uniform vec3 viewPos;
layout(location = 20) uniform sampler2D textures[NUM_TEX];

vec3 computeLighting(vec3 lightPos, vec3 lightColor, vec3 baseColor)
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    return (ambient + diffuse + specular) * baseColor;
}

void main()
{
    vec4 texColor = texture(textures[spheres[sphereIndex].textureInd], TexCoord);
    vec3 baseColor = texColor.rgb * spheres[sphereIndex].objectColor.rgb;
    vec3 result = baseColor;
    // result += computeLighting(lightPos, lightColor, baseColor);
    // result += computeLighting(lightPos2, lightColor2, baseColor);
    FragColor = vec4(result, texColor.a);
}
