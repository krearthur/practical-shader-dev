#version 410 

uniform sampler2D mainTex;
uniform sampler2D specMap;
uniform vec3 lightDir;
uniform vec3 lightCol;
uniform vec3 ambientCol;
uniform vec3 meshCol;
uniform vec3 cameraPos;

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragWorldPos;
out vec4 outCol;

void main() 
{
    vec3 normal = normalize(fragNormal);
    
    vec3 viewDir = normalize(cameraPos - fragWorldPos);
    
    vec4 texCol = texture(mainTex, fragUV);
    
    float diffAmt = max(0.0, dot(normal, lightDir));
    vec3 diffCol = texCol.rgb * lightCol * diffAmt;

    vec4 specVal = texture(specMap, fragUV);

    // phong specular
    // vec3 refl = reflect(-lightDir, normal);
    // float specAmt = max(0.0, dot(refl, viewDir)) * specVal.r;

    // blinn-phong specular
    vec3 halfVec = normalize(viewDir + lightDir);
    float specAmt = max(0.0, dot(halfVec, normal)) * specVal.r;
    float specBright = pow(specAmt, 32.0);
    vec3 specCol = lightCol * specBright;

    vec3 ambient = texCol.rgb * ambientCol;

    outCol = vec4(diffCol + specCol + ambient, texCol.a);
}