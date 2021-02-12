#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 tan;
layout (location = 2) in vec3 nrm;
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat3 normalMatrix; // transforms normals ( or any other vector) from object space to world space 
uniform mat4 model;

out vec2 fragUV;
out vec3 fragNormal;
out vec3 fragWorldPos;
out mat3 TBN; // Tangent-Bitangent-Normal matrix, used to transform the vectors of normal-maps from tangent-space to world space

void main()
{
    // define the TBN (tangent-bitangent-normal) vectors and multiply by normalMatrix 
    // to transform them to world space
    vec3 T = normalize( normalMatrix * tan.xyz );
    vec3 B = normalize( normalMatrix * cross(T, nrm) );
    vec3 N = normalize( normalMatrix * nrm );
    TBN = mat3(T, B, N);
    

    gl_Position = mvp * vec4(pos, 1.0);
    fragNormal = normalMatrix * nrm;
    fragUV = vec2(uv.x, 1 - uv.y);
    fragWorldPos = (model * vec4(pos, 1.0)).xyz;
}