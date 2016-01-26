#version 440

in vec3 posL;
in vec3 normL;
in vec2 intexC;

uniform mat4 gWMtx;
uniform mat4 gWITMtx;
uniform mat4 gWVPMtx;

out vec3 posW;
out vec3 normW;
out vec2 outtexC;

void main (void) {
    posW = mul(vec4(posL,1.0f),gWMtx).xyz;
    normW = mul(vec4(normL,0.0f),gWITMtx).xyz;
    outtexC = intexC;
    
    gl_Position = mul(vec4(posL,1.0f),gWVPMtx);
}