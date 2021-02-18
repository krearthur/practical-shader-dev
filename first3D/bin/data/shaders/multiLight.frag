#version 410

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float radius;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float hardness;
    float cutoff;
};

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 2
#define NUM_SPOT_LIGHTS 2

uniform DirectionalLight directionalLights[NUM_DIR_LIGHTS];
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];
uniform sampler2D diffuseTex;
uniform sampler2D specMap;
uniform sampler2D normalMap;
uniform samplerCube envMap;
uniform vec3 cameraPos;
uniform vec3 ambientCol;

in vec3 fragNrm;
in vec3 fragWorldPos;
in vec2 fragUV;
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

vec3 sumDirLights(vec3 diffuseColor, vec3 envSample, float specMask, vec3 viewDir, vec3 nrm) 
{
    vec3 sumCol = vec3(0, 0, 0);
    for (int i = 0; i < NUM_DIR_LIGHTS; ++i)
    {
        DirectionalLight light = directionalLights[i];
        // Environment Reflection
        vec3 envCol = mix(light.color, envSample + light.color * 0.5, 0.5);

        // Diffuse
        float diffAmt = diffuse(light.direction, nrm);
        vec3 diffCol = diffuseColor * envCol * diffAmt;

        // Specular
        float specAmt = specular(light.direction, viewDir, nrm, 4.0);
        vec3 specCol = specMask * envCol * specAmt;

        sumCol += diffCol + specCol;
    }
    
    return sumCol;
}

vec3 sumPointLights(vec3 diffuseColor, vec3 envSample, float specMask, vec3 viewDir, vec3 nrm)
{
    vec3 sumCol;
    for (int i = 0; i < NUM_POINT_LIGHTS; ++i) 
    {
        PointLight light = pointLights[i];
        
        // Environment Reflection
        vec3 envCol = mix(light.color, envSample + light.color * 0.5, 0.5);

        // Point light calculation
        vec3 toLight = light.position - fragWorldPos;
        vec3 lightDir = normalize(toLight);
        float distToLight = length(toLight);
        float falloff = 1.0 - (distToLight / light.radius);

        // Diffuse 
        float diffAmt = diffuse(lightDir, nrm) * falloff;
        vec3 diffCol = diffuseColor * envCol * diffAmt;

        // Spec
        float specAmt = specular(lightDir, viewDir, nrm, 4.0) * falloff;
        vec3 specCol = specMask * envCol * specAmt;
        
        sumCol += diffCol + specCol;
    }
    return sumCol;
}

vec3 sumSpotLights(vec3 diffuseColor, vec3 envSample, float specMask, vec3 viewDir, vec3 nrm)
{
    vec3 sumCol;
    for (int i = 0; i < NUM_SPOT_LIGHTS; ++i) 
    {
        SpotLight light = spotLights[i];
        
        // Reflection from environment map
        vec3 envCol = mix(light.color, envSample + light.color * 0.5, 0.5);

        // Spot light calculation
        vec3 toLight = light.position - fragWorldPos;
        vec3 lightDir = normalize(toLight);
        float falloff = 0.0;
        float cosAngle = max(0.0, dot(-lightDir, light.direction));
        if (cosAngle > light.cutoff) falloff = 1; // hard falloff edge
        //falloff = (cosAngle - light.cutoff) / (1.0 - light.cutoff); // soft edge, shortened equation with linear falloff
        //falloff = min(1.0, ((cosAngle / light.cutoff) -1 ) / (light.hardness * ((1.0 / light.cutoff) - 1))); // soft edge, with more 100% light in the center 
        //falloff = (max(0.0, cos( falloff * 4 * TAU )) + falloff) * 0.5;

        // Diffuse 
        float diffAmt = diffuse(lightDir, nrm) * falloff;
        vec3 diffCol = diffuseColor * envCol * diffAmt;

        // Spec
        float specAmt = specular(lightDir, viewDir, nrm, 4.0) * falloff;
        vec3 specCol = specMask * envCol * specAmt;
        
        sumCol += diffCol + specCol;
    }
    return sumCol;
}

void main() 
{
    // Normals
    vec3 nrm = texture(normalMap, fragUV).rgb;
    nrm = normalize(nrm * 2.0 - 1.0);
    nrm = normalize(TBN * nrm);
    vec3 viewDir = normalize( cameraPos - fragWorldPos );
    
    // Diffuse
    vec3 diffuseColor = texture(diffuseTex, fragUV).rgb;
    
    // Specular mask 
    float specMask = texture(specMap, fragUV).x;
    
    // Reflection from environment
    vec3 envReflection = texture(envMap, reflect(-viewDir, nrm)).xyz;
    
    // Declare final color
    vec3 finalColor = vec3(0, 0, 0);
    finalColor += sumDirLights(diffuseColor, envReflection, specMask, viewDir, nrm);
    finalColor += sumPointLights(diffuseColor, envReflection, specMask, viewDir, nrm);
    finalColor += sumSpotLights(diffuseColor, envReflection, specMask, viewDir, nrm);
    
    // Ambient
    vec3 ambient = diffuseColor * ambientCol;

    outCol = vec4(finalColor + ambient, 1.0);
}