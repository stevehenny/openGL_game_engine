#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) uniform vec3 textColor; 
layout(location = 2) in vec2 TexCoords;
layout(location = 3) uniform sampler2D text;

void main() {
    float alpha = texture(text, TexCoords).r;
    FragColor = vec4(textColor, alpha);
}

