#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec2 TexCoord;

layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
