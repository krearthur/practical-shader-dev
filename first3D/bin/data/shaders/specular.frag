#version 410 

uniform sampler2D mainTex;
uniform sampler2D specMap;
uniform sampler2D normalMap;
uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 ambientCol;
uniform vec3 meshCol;
uniform vec3 cameraPos;

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragWorldPos;
in mat3 TBN;

out vec4 outCol;

void main() 
{
    vec3 sampledNrm = texture(normalMap, fragUV).xyz;
    sampledNrm = normalize(sampledNrm * 2.0 - 1.0); // convert the color ranges that go from 0 to 1, to a range from -1 to 1
    sampledNrm = normalize(TBN * sampledNrm); // converting the Tangent Space normal to World Space
    

    vec3 viewDir = normalize(cameraPos - fragWorldPos);
    vec3 halfVec = normalize(viewDir + lightDir);

    vec4 texCol = texture(mainTex, fragUV);
    
    float diffAmt = max(0.0, dot(sampledNrm, lightDir));
    vec3 diffCol = texCol.rgb * lightCol * diffAmt;

    // blinn-phong specular
    float specAmt = max(0.0, dot(halfVec, sampledNrm)) ;
    float specBright = pow(specAmt, 32.0);
    vec3 specCol = texture(specMap, fragUV).r * lightCol * specBright;

    vec3 ambient = texCol.rgb * ambientCol;

    outCol = vec4(diffCol + specCol + ambient, texCol.a);
}