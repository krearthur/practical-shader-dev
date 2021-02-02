#version 410

uniform sampler2D parrotTex;

in vec2 fragUV;
out vec4 outCol;

void main()
{
    // Get one pixel out of the texture, therefore it needs the uv coordinates
    // this process is called "sampling"
    outCol = texture(parrotTex, fragUV);
}