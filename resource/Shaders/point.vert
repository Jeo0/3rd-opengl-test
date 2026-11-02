#version 450 core
// vertex dots: the same VAO as the model, drawn with GL_POINTS instead of GL_TRIANGLES.
// each vertex becomes a square sprite gl_PointSize pixels wide (needs GL_PROGRAM_POINT_SIZE).

layout (location = 0) in vec3 a_pos;

uniform mat4 u_camMatrix;
uniform mat4 u_modelPos;
uniform vec3 u_camPos;
uniform float u_pointSize;      // dot diameter in pixels
uniform float u_pixelAngle;     // 2*tan(fov/2)/viewportHeight, set by Scene

void main()
{
    vec3 m_world = vec3(u_modelPos * vec4(a_pos, 1.0));

    // a sprite has ONE depth for its whole square, so faces sloping toward the camera
    // right next to the vertex would cover half the dot. slide the vertex toward the
    // camera along its own view ray by the dot's radius: same spot on screen, but now
    // it sits in front like a little ball resting on the surface.
    vec3 m_toCam = u_camPos - m_world;
    float m_dist = length(m_toCam);
    float m_radiusWorld = 0.5 * u_pointSize * u_pixelAngle * m_dist;
    m_world += (m_toCam / m_dist) * min(m_radiusWorld, 0.5 * m_dist);

    gl_Position = u_camMatrix * vec4(m_world, 1.0);

    // constant on-screen size, good for inspecting
    gl_PointSize = u_pointSize;

    // alternative: dots shrink with distance like real objects (raise u_pointSize to ~40)
    // gl_PointSize = u_pointSize / gl_Position.w;
}

