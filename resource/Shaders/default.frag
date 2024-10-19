#version 330 core

in vec3 cololor;          

out vec4 FragColor;

void main() {
    FragColor = vec4(cololor, 1.0f);

}