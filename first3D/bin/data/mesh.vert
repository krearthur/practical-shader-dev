#version 410

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat3 normalMatrix; // transforms normals from object space to world space 
uniform mat4 model;

out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main()
{
    gl_Position = mvp * vec4(pos, 1.0);
    fragNormal = normalMatrix * normal;
    fragUV = vec2(uv.x, 1.0 - uv.y);
    fragWorldPos = (model * vec4(pos, 1.0)).xyz;
}