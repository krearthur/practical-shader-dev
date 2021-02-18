#version 410 

uniform sampler2D normalMap;
uniform samplerCube envMap;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 lightConeDir;
uniform float lightCutoff;
uniform vec3 ambientCol;
uniform vec3 cameraPos;
uniform float alpha;

in vec2 fragUV;
in vec2 fragUV2;

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
    // Combine the two floating UVs for water normal
    vec3 nrm = texture(normalMap, fragUV).xyz;
    nrm = normalize(nrm * 2.0 - 1.0); // convert the color ranges that go from 0 to 1, to a range from -1 to 1
    vec3 nrm2 = texture(normalMap, fragUV2).xyz;
    nrm2 = normalize(nrm2 * 2.0 - 1.0);
    nrm = normalize(TBN * (nrm + nrm2));

    // Reflection col    
    vec3 viewDir = normalize(cameraPos - fragWorldPos);
    vec3 envSample = texture(envMap, reflect(-viewDir, nrm)).xyz;
    vec3 envCol = mix(lightCol, envSample + lightCol * 0.5, 0.5);

    // Spot light calculation
    vec3 toLight = lightPos - fragWorldPos;
    vec3 lightDir = normalize(toLight);
    float falloff = 0.0;
    float cosAngle = max(0.0, dot(-lightDir, lightConeDir));
    //if (cosAngle > lightCutoff) falloff = 1; // hard falloff edge
    //falloff = (cosAngle - lightCutoff) / (1.0 - lightCutoff); // soft edge, shortened equation with linear falloff
    falloff = min(1.0, ((cosAngle / lightCutoff) -1 ) / (0.5 * ((1.0 / lightCutoff) - 1))); // soft edge, with more 100% light in the center
    //falloff = (max(0.0, cos( falloff * 4 * TAU )) + falloff) * 0.5; // light-ripples :P
    
    // Diffuse 
    float diffAmt = diffuse(lightDir, nrm) * falloff;
    vec3 albedo = vec3(0.4, 0.4, 0.6);
    vec3 diffCol = albedo * envCol * diffAmt;

    // Spec
    float specAmt = specular(lightDir, viewDir, nrm, 16.0) * falloff;
    vec3 specCol = envCol * specAmt;

    // Ambient
    vec3 ambient = albedo.rgb * ambientCol;

    // Final
    outCol = vec4(diffCol + specCol + ambient, alpha);
}