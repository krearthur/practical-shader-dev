#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

out vec2 fragUV;

void main() 
{
    gl_Position = vec4(pos, 1.0);
    
    // we have to flip the uv coordinates vertically
    // because openGL thinks that images start at the bottom row
    // but almost all image format starts at the top row. (WTF?! XD)
    fragUV = vec2(uv.x, 1.0 - uv.y);

}