#version 460 core

// These are outputs from the vertex shader
layout(location = 0) in vec3 Normal;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec2 TexCoord;

// Output color
layout(location=0) out vec4 FragColor;

// Uniforms
layout(location = 20) uniform vec3 objectPos;
layout(location = 21) uniform float mass;
layout(location = 22) uniform sampler2D texture1;

void main()
{
    // Compute distance-based color to visualize gravity effect
    float dist = length(FragPos - objectPos);
    float intensity = clamp(1.0 / (dist + 0.1), 0.0, 1.0);

    vec3 texColor = texture(texture1, TexCoord).rgb;
    vec3 color = mix(texColor, vec3(1.0, 0.2, 0.2), intensity);

    FragColor = vec4(color, 1.0);
}
