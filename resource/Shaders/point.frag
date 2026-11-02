#version 450 core

out vec4 FragColor;

uniform vec3 u_pointColor;

void main() {
    // gl_PointCoord goes 0..1 across the square sprite; recenter it to -1..1
    vec2 m_fromCenter = gl_PointCoord * 2.0 - 1.0;
    float m_radius = length(m_fromCenter);

    if (m_radius > 1.0) discard;    // cut the square's corners -> round dot

    // dark rim so the dot stays readable on light and dark surfaces alike
    float m_rim = smoothstep(0.65, 0.8, m_radius);
    FragColor = vec4(mix(u_pointColor, vec3(0.0), m_rim), 1.0);
}

