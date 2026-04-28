#version 460 core

in vec3 cololor;          
in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D tex0;

void main() {
    // FragColor = vec4(cololor, 1.0f);
    FragColor = texture(tex0, textureCoordinates);

}
