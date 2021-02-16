#version 410

uniform sampler2D mainTex;
uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 meshCol;
uniform vec3 cameraPos;

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outCol;

void main()
{
    vec3 normal = normalize(fragNormal);

    vec3 toCam = normalize(cameraPos - fragWorldPos);
    float rimAmt = 1.0 - max(0.0, dot(normal, toCam));
    rimAmt = pow(rimAmt, 2);

    float lightAmt = max(0.0, dot(normal, lightDir));
    vec3 fragLight = lightCol * lightAmt;

    vec4 texCol = texture(mainTex, fragUV);  
    
    outCol = vec4(texCol.rgb * meshCol * fragLight + rimAmt, texCol.a);
}