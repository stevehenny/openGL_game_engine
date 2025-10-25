#version 460 core
layout(location = 0) in vec3 Normal;
layout(location = 1) in vec3 FragPos;

layout(location = 0) out vec4 FragColor;

layout(location = 5) uniform vec3 objectColor;
layout(location = 6) uniform vec3 lightColor;
layout(location = 7) uniform vec3 lightPos;
layout(location = 8) uniform vec3 lightColor2;
layout(location = 9) uniform vec3 lightPos2;
layout(location = 10) uniform vec3 viewPos;

vec3 computeLighting(vec3 lightPos, vec3 lightColor)
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

    return (ambient + diffuse + specular) * objectColor;
}

void main()
{
    vec3 result = computeLighting(lightPos, lightColor);
    result += computeLighting(lightPos2, lightColor2);
    FragColor = vec4(result, 1.0);
}
