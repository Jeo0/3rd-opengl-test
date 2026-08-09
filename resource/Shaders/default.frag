#version 460 core

in vec3 v_cololor;          
in vec2 v_textureCoordinates;

out vec4 FragColor;

// uniform sampler2D tex0;
uniform sampler2D u_diffuse_tex_type0;
uniform vec3 u_camPos;

void main() {
    // FragColor = vec4(cololor, 1.0f);
    // FragColor = texture(u_camPos, v_textureCoordinates); // previous REMOVE
    FragColor = texture(u_diffuse_tex_type0, v_textureCoordinates);

}
