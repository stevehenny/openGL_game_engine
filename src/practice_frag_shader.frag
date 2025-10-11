#version 460 core

in vec3 ourColor;
out vec4 FragColor;

uniform mat4 colorTransform;
void main(){
   FragColor =  colorTransform * vec4(ourColor, 1.0);
}
