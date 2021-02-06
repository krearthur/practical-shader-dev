#version 410

layout (location = 0) in vec3 pos; // vertex position
layout (location = 3) in vec2 uv; // vertex uv value

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec2 size; // of one frame in the sprite sheet
uniform vec2 offset;

out vec2 fragUV;

void main()
{
    // -- Apply three transformation matrices (4x4) to get our model on screen --
    // read from right to left, so first we have the vertex pos in OBJECT SPACE.
    // Then the model matrix gets applied to convert the positions into WORLD SPACE,
    // then the view matrix gets applied to convert the positions into VIEW SPACE (which is the camera relative space)
    // and finally multiplying by the projection matrix, 
    // to convert the positions to CLIP SPACE (aka normalized device coordinates), which the GPU needs to render to screen.
    gl_Position = proj * view * model * vec4(pos, 1.0);

    fragUV = vec2(uv.x, 1.0 - uv.y) * size + (offset * size);
}