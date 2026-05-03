#version 460 core
// remember vertices in main contains 6 columns and 4 rows (6 data, for 4 points)
layout (location = 0) in vec3 aPos;         // in the array, every 3 contiguous elements are of position data being inputted
layout (location = 1) in vec3 aColor;       // same goes here. Except its for color
layout (location = 2) in vec2 aTex;

out vec3 cololor;            // pass it to the fragment as vertex shader does not care about colors
out vec2 textureCoordinates;

uniform float scale;

// 3d tings 
uniform mat4 Umodel;
uniform mat4 Uview;
uniform mat4 Uprojection;

void main()
{
    gl_Position = Uprojection * Uview * Umodel
                // * vec4(aPos, 1.0f);
                * vec4( aPos.x * (1 + scale), 
                        aPos.y * (1 + scale), 
                        aPos.z * (1 + scale), 1.0);
    cololor = aColor;
    textureCoordinates = aTex;
}
