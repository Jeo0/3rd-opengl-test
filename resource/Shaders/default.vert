
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
/*
#version 330 core
in vec3 cololor;          
out vec4 FragColor;

void main() {
    FragColor = vec4(cololor, 1.0f);
}
*/
