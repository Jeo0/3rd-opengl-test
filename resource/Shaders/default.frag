#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.8, 0.3, 0.3, 1.0); // red-ish
}
/*
#version 330 core
in vec3 cololor;          
out vec4 FragColor;

void main() {
    FragColor = vec4(cololor, 1.0f);
}
*/
