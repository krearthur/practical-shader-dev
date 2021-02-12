#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 tan;
layout (location = 2) in vec3 nrm;
layout (location = 3) in vec2 uv;

uniform mat4 mvp;
uniform mat3 normalMatrix; // transforms normals ( or any other vector) from object space to world space 
uniform mat4 model;
uniform float time;

out vec2 fragUV;
out vec2 fragUV2;

out vec3 fragNormal;
out vec3 fragWorldPos;
out mat3 TBN; // Tangent-Bitangent-Normal matrix, used to transform the vectors of normal-maps from tangent-space to world space

void main()
{
    float t = time * 0.05;
    float t2 = time * 0.02;
    fragUV = vec2(uv.x + t * 0.5f, uv.y) * 3.0f ;
    fragUV2 = vec2(uv.x + t2 * 0.5f, uv.y - t2 * 0.5f) * 2.0f ;

    // define the TBN (tangent-bitangent-normal) vectors and multiply by normalMatrix 
    // to transform them to world space
    vec3 T = normalize( normalMatrix * tan.xyz );
    vec3 B = normalize( normalMatrix * cross(T, nrm) );
    vec3 N = normalize( normalMatrix * nrm );
    TBN = mat3(T, B, N);
    
    gl_Position = mvp * vec4(pos, 1.0);
    fragNormal = normalMatrix * nrm;
    
    fragWorldPos = (model * vec4(pos, 1.0)).xyz;
}