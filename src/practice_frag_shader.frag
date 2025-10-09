#version 460 core

in vec3 ourColor;
out vec4 FragColor;

uniform vec4 colorTransform;
void main(){
   FragColor =  vec4(ourColor, 1.0);
}
