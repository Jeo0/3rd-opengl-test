#version 450 core
// stage 2 of 3 for the model: runs once per TRIANGLE, between vertex and fragment.
// it passes the triangle through unchanged, but stamps each corner with a barycentric
// coordinate: (1,0,0), (0,1,0), (0,0,1). after interpolation, every pixel knows how
// far it is from each of the triangle's three edges -> that's what draws the wireframe.
//
// why not just glPolygonMode(GL_LINE) + glLineWidth(5)? in a 4.6 core context, line
// widths above 1 are deprecated and many drivers clamp them to 1 pixel.

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 crntPos;
    vec3 color;
    vec2 textureCoordinates;
} gs_in[];

// same names default.frag already reads
out vec3 u_crntPos;
out vec3 v_cololor;
out vec2 v_textureCoordinates;

// noperspective: interpolate linearly in SCREEN space, so the line has the same pixel
// width along its whole length instead of thinning out with depth
noperspective out vec3 g_bary;

void main()
{
    vec3 m_corners[3] = vec3[](vec3(1.0, 0.0, 0.0),
                               vec3(0.0, 1.0, 0.0),
                               vec3(0.0, 0.0, 1.0));

    for (int iii = 0; iii < 3; iii++) {
        gl_Position          = gl_in[iii].gl_Position;
        u_crntPos            = gs_in[iii].crntPos;
        v_cololor            = gs_in[iii].color;
        v_textureCoordinates = gs_in[iii].textureCoordinates;
        g_bary               = m_corners[iii];
        EmitVertex();
    }
    EndPrimitive();
}

