#version 410 core 

uniform vec3 objectColor;
uniform vec3 light;

out vec4 FragColor;
void main(){
  FragColor = vec4(light * objectColor, 1.0);
}
