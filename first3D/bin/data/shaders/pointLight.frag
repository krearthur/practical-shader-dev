#version 410

uniform vec3 meshCol;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform float lightRadius;

in vec3 fragNrm;
in vec3 fragWorldPos;
out vec4 outCol;

void main() {
    vec3 normal = normalize(fragNrm);
    vec3 toLight = lightPos - fragWorldPos;
    vec3 lightDir = normalize(toLight);
    float distToLight = length(toLight);
    
    float falloff = max(0.0, 1.0 - (distToLight / lightRadius));
    vec3 adjLightCol = lightCol * falloff;
    float finalBright = max(0, dot(lightDir, normal)) * ajdLightCol;
    
    outCol = vec4(finalBright * meshCol, 1.0); 
}
