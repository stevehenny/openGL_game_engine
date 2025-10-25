#version 460 core 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 2) uniform mat4 projection;
layout (location = 3) uniform mat4 view;
layout (location = 4) uniform mat4 model;

layout (location = 5) out vec2 TexCoord;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);  
    TexCoord = aTexCoord;
}

