
#version 460 core

layout(location = 0) in vec3 ourColor;
layout(location = 1) in vec2 TexCoord;
layout(location = 0) out vec4 FragColor;
layout(binding = 0) uniform sampler2D outTexture;

void main()
{
    vec4 texColor = texture(outTexture, TexCoord);
    FragColor = texColor * vec4(ourColor, 1.0); 
}

