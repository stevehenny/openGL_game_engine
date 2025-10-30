#version 460 core

layout(location = 0) in vec2 TexCoord;
layout(location = 0) out vec4 FragColor;
layout(binding = 0) uniform sampler2D outTexture;

void main()
{
    FragColor = texture(outTexture, TexCoord);

}
