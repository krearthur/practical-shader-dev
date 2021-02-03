#version 410

layout (location = 0) in vec3 pos; // vertex position
layout (location = 3) in vec2 uv; // vertex uv value

uniform vec3 position;
uniform vec2 size; // of one frame in the sprite sheet
uniform vec2 offset;

out vec2 fragUV;

void main()
{
    gl_Position = vec4(pos + position, 1.0);
    fragUV = vec2(uv.x, 1.0 - uv.y) * size + (offset * size);
}