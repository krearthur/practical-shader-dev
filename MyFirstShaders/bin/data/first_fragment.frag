#version 410

in vec4 fragCol;
out vec4 outColor;

void main() 
{
    float prec = 25;
    float r = int(fragCol.r * prec) / prec;
    float g = int(fragCol.g * prec) / prec;
    float b = int(fragCol.b * prec) / prec;
    
    vec4 simpCol = vec4(r,g,b, fragCol.a);
    
    outColor = simpCol;
}