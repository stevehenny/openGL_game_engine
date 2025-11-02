#version 460 core




layout(location = 0) in vec4 vertex;
layout(location = 1) out vec2 TexCoords; // outputs to fragment shader
layout(location = 0) uniform mat4 projection; // location 0


void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}

