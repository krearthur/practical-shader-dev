#version 410

uniform sampler2D parrotTex;
uniform sampler2D checkerBoardTex;

in vec2 fragUV;
out vec4 outCol;

void main() 
{
    vec4 parrot = texture(parrotTex, fragUV);
    vec4 checker = texture(checkerBoardTex, fragUV * 3.0f);

    outCol = mix(checker, parrot, checker.r);
}