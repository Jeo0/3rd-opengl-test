#version 450 core
// stage 1 of 3 for the model: runs once per vertex.
// job: move the vertex into clip space (gl_Position) and hand the per-vertex data onward.

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_tex;

// bundled into a block because the geometry shader re-emits these under the names the
// fragment shader already uses (u_crntPos, v_cololor, ...); a block avoids the name clash
out VS_OUT {
    vec3 crntPos;               // world-space position, for lighting / facing
    vec3 color;
    vec2 textureCoordinates;
} vs_out;

uniform mat4 u_camMatrix;       // projection * view
uniform mat4 u_modelPos;        // this mesh's own place in the scene; look into Mesh::SetPosition

void main()
{
    vec4 m_world = u_modelPos * vec4(a_pos, 1.0);

    vs_out.crntPos = m_world.xyz;
    vs_out.color = a_color;
    vs_out.textureCoordinates = a_tex;

    gl_Position = u_camMatrix * m_world;
}
