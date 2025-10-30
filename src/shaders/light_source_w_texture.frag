#version 460 core

layout(location = 0) in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

layout(location = 13) uniform vec3 lightColor;
layout(location = 14) uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    FragColor = vec4(lightColor, 1.0f) * texColor;
}
