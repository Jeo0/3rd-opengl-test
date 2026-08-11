#version 460 core
// remember vertices in main contains 6 columns and 4 rows (6 data, for 4 points)
layout (location = 0) in vec3 a_pos;         // in the array, every 3 contiguous elements are of position data being inputted
layout (location = 1) in vec3 a_color;       // same goes here. Except its for color
layout (location = 2) in vec2 a_tex;

out vec3 v_cololor;            // pass it to the fragment as vertex shader does not care about colors
out vec2 v_textureCoordinates;

// uniform float scale; // REMOVE

// 3d tings 
// uniform mat4 Umodel;
// uniform mat4 Uview;
// uniform mat4 Uprojection;

uniform mat4 u_camMatrix;
uniform mat4 u_modelPos;   // this mesh's own place in the scene - see Mesh::SetPosition

void main()
{
    // gl_Position = Uprojection * Uview * Umodel
    //             * vec4(a_pos, 1.0f);
                // * vec4(a_pos * (1 + scale), 1.0f);
                // * vec4( a_pos.x * (1 + scale), 
                //         a_pos.y * (1 + scale), 
                //         a_pos.z * (1 + scale), 1.0);
    gl_Position = u_camMatrix * u_modelPos * vec4(a_pos, 1.0);
    v_cololor = a_color;
    v_textureCoordinates = a_tex;
}
