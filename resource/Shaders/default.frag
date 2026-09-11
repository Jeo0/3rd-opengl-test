#version 460 core

in vec3 u_crntPos;
in vec3 v_cololor;
in vec2 v_textureCoordinates;

out vec4 FragColor;

uniform sampler2D u_diffuse_tex_type1;
uniform vec3 u_camPos;

void main() {
    // added with a fading effect not perpendicular to the camera

    vec4 texColor = texture(u_diffuse_tex_type1, v_textureCoordinates);

    // flat per-triangle normal, derived from screen-space derivatives of world pos
    vec3 dx = dFdx(u_crntPos);
    vec3 dy = dFdy(u_crntPos);
    vec3 normal = normalize(cross(dx, dy));

    vec3 viewDir = normalize(u_camPos - u_crntPos);
    float facing = max(dot(normal, viewDir), 0.0);

    float edge = 1.0 - facing;
    edge = smoothstep(0.55, 0.85, edge); // tweak these two to control edge width/softness

    vec3 finalColor = mix(texColor.rgb, vec3(0.0), edge);
    FragColor = vec4(finalColor, texColor.a);
}
