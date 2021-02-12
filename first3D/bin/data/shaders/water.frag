#version 410 

uniform sampler2D normalMap;
uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 ambientCol;
uniform vec3 cameraPos;
uniform float alpha;

in vec2 fragUV;
in vec2 fragUV2;

in vec3 fragWorldPos;
in mat3 TBN;

out vec4 outCol;

void main() 
{
    vec3 nrm = texture(normalMap, fragUV).xyz;
    nrm = normalize(nrm * 2.0 - 1.0); // convert the color ranges that go from 0 to 1, to a range from -1 to 1
    
    vec3 nrm2 = texture(normalMap, fragUV2).xyz;
    nrm2 = normalize(nrm2 * 2.0 - 1.0);

    nrm = normalize(TBN * (nrm + nrm2));

    vec3 viewDir = normalize(cameraPos - fragWorldPos);
    vec3 halfVec = normalize(viewDir + lightDir);

    float diffAmt = max(0.0, dot(nrm, lightDir));
    vec3 oceanBlue = vec3(0.3, 0.3, 0.4);
    vec3 diffCol = oceanBlue * lightCol * diffAmt;
    
    // blinn-phong specular
    float specAmt = max(0.0, dot(halfVec, nrm)) ;
    float specBright = pow(specAmt, 512.0);
    vec3 specCol = lightCol * specBright;

    vec3 ambient = ambientCol;

    outCol = vec4(diffCol + specCol + ambient, alpha);
}