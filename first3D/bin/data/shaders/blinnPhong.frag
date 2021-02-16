#version 410 

uniform sampler2D mainTex;
uniform sampler2D specMap;
uniform sampler2D normalMap;
uniform samplerCube envMap;
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

float diffuse(vec3 lightDir, vec3 nrm) 
{
    float diffAmt = max(0.0, dot(nrm, lightDir));
    return diffAmt;
}

float specular(vec3 lightDir, vec3 viewDir, vec3 nrm, float shininess)
{
    vec3 halfVec = normalize(viewDir + lightDir);
    float specAmt = max(0.0, dot(halfVec, nrm));
    return pow(specAmt, shininess);
}

void main() 
{
    // Normals
    vec3 nrm = texture(normalMap, fragUV).xyz;
    nrm = normalize(nrm * 2.0 - 1.0); // convert the color ranges that go from 0 to 1, to a range from -1 to 1
    nrm = normalize(TBN * nrm); // converting the Tangent Space normal to World Space
    
    // Reflection
    vec3 viewDir = normalize(cameraPos - fragWorldPos);
    vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
    vec3 envCol = mix(lightCol, envSample + lightCol * 0.5, 0.5);

    // Diffuse
    float diffAmt = diffuse(lightDir, nrm);
    vec4 albedo = texture(mainTex, fragUV);
    vec3 diffCol = albedo.xyz * envCol * diffAmt;
    
    // Specular
    float specAmt = specular(lightDir, viewDir, nrm, 8.0);
    vec3 specCol = texture(specMap, fragUV).r * envCol * specAmt;

    // Ambient
    vec3 ambCol = albedo.rgb * ambCol;

    // Final
    outCol = vec4(diffCol + specCol + ambCol, albedo.a);
}