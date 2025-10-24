#version 410 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 lightPos2;
uniform vec3 lightColor2;

vec3 lightingVec(vec3 lightPos, vec3 lightColor){

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


void main() {

    vec3 result = lightingVec(lightPos, lightColor);
    result += lightingVec(lightPos2, lightColor2);
    FragColor = vec4(result, 1.0);
}

