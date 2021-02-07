#version 410 

uniform sampler2D mainTex;
uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 meshCol;

in vec2 fragUV;
in vec3 fragNormal;
out vec4 outCol;

void main() 
{
    vec3 normal = normalize(fragNormal);

    float lightAmt = max(0.0, dot(normal, lightDir));
    vec3 fragLight = lightCol * lightAmt;

    vec4 texCol = texture(mainTex, fragUV);

    outCol = vec4(texCol.rgb * meshCol * fragLight, texCol.a);
}