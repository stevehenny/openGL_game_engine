#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 Normal;
layout(location = 1) out vec3 FragPos;
layout(location = 2) out vec2 TexCoord;

layout(location = 2) uniform mat4 model;
layout(location = 3) uniform mat4 view;
layout(location = 4) uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * worldPos;
}
