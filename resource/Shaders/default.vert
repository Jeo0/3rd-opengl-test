#version 330 core
// remember vertices in main contains 6 columns and 4 rows (6 data, for 4 points)
layout (location = 0) in vec3 aPos;         // in the array, every 3 contiguous elements are of position data being inputted
layout (location = 1) in vec3 aColor;       // same goes here. Except its for color

out vec3 cololor;            // pass it to the fragment as vertex shader does not care about colors

uniform float scale;


void main()
{
    gl_Position = vec4( aPos.x * (1 + scale), 
                        aPos.y * (1 + scale), 
                        aPos.z * (1 + scale), 1.0);
    cololor = aColor;
}