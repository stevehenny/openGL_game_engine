#version 460 core

layout(location = 2) in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;
layout(location = 5) uniform vec3 lightColor;
layout(location = 6) uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    vec3 baseColor = texColor.rgb * lightColor;
    FragColor = vec4(baseColor, texColor.a);
}
