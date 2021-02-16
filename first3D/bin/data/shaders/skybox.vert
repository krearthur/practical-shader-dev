#version 410

layout( location = 0) in vec3 pos;

uniform mat4 mvp;
out vec3 fromCam;

void main()
{
    fromCam = pos;

    // Set z to the value of w, so when the "Perspective Divide" step happens, 
    // which divides x, y, z by w to project the points into the clipping space.
    // dividing a / a is always 1 to the points will always be at the farthest point inside the clipping plane,
    // so they will be rendered last. VERY COOL TRICK!! NIFTY
    gl_Position = (mvp * vec4(pos, 1.0)).xyww;


}