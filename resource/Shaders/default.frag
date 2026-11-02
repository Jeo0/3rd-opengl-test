#version 450 core
// stage 3 of 3 for the model: runs once per PIXEL the triangle covers.

in vec3 u_crntPos;
in vec3 v_cololor;
in vec2 v_textureCoordinates;
noperspective in vec3 g_bary;   // from default.geom; 0 on an edge, 1 at the opposite corner

out vec4 FragColor;

uniform sampler2D u_diffuse_tex_type1;
uniform bool u_hasDiffuse;      // set by Mesh::Draw; an unbound sampler reads as black
uniform vec3 u_camPos;

uniform float u_edgeWidth;      // wireframe thickness in pixels, 0 = off
uniform vec3  u_edgeColor;

void main() {
    vec4 m_base = u_hasDiffuse ? texture(u_diffuse_tex_type1, v_textureCoordinates)
                               : vec4(0.75, 0.75, 0.75, 1.0);

    // flat per-triangle normal, derived from screen-space derivatives of world pos
    vec3 m_dx = dFdx(u_crntPos);
    vec3 m_dy = dFdy(u_crntPos);
    vec3 m_normal = normalize(cross(m_dx, m_dy));

    // darken faces that turn away from the camera (this is NOT the mesh edges --
    // it shades whole triangles by viewing angle, which is why it never looked like lines)
    vec3 m_viewDir = normalize(u_camPos - u_crntPos);
    float m_facing = max(dot(m_normal, m_viewDir), 0.0);
    float m_grazing = smoothstep(0.55, 0.85, 1.0 - m_facing);
    vec3 m_color = mix(m_base.rgb, vec3(0.0), m_grazing);

    // ---- wireframe ----
    // fwidth(g_bary) = how much g_bary changes from this pixel to the next one, so
    // g_bary / fwidth(g_bary) = distance to each edge measured in pixels
    vec3 m_pixelsToEdge = g_bary / max(fwidth(g_bary), vec3(1e-6));
    float m_nearestEdge = min(min(m_pixelsToEdge.x, m_pixelsToEdge.y), m_pixelsToEdge.z);

    // an inner edge is shared by two triangles and each paints its own half,
    // so each side only needs half the requested width
    float m_halfWidth = 0.5 * u_edgeWidth;
    // the +-0.5 px ramp is the antialiasing; drop it for hard pixel edges
    float m_line = 1.0 - smoothstep(m_halfWidth - 0.5, m_halfWidth + 0.5, m_nearestEdge);
    m_line *= step(0.001, u_edgeWidth);

    m_color = mix(m_color, u_edgeColor, m_line);
    FragColor = vec4(m_color, m_base.a);
}
