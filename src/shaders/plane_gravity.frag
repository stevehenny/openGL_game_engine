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
layout(location = 23) uniform bool forceWhite;

void main()
{
    // Compute distance-based color to visualize gravity effect
      FragColor = vec4(1.0);
}
