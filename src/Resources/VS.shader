#version 440

in vec3 posL;
in vec3 normL;
in vec2 intexC;

uniform mat4 gWMtx;
uniform mat4 gWITMtx;
uniform mat4 gWVPMtx;
uniform mat3 gTexMtx;

out vec3 posW;
out vec3 normW;
out vec2 outtexC;

void main (void) {
    posW = (vec4(posL,1.0f) * gWMtx).xyz;
    normW = (vec4(normL,0.0f) * gWITMtx).xyz;
    outtexC = (vec3(intexC,1.0f) * gTexMtx).xy;
    
    gl_Position = (vec4(posL,1.0f) * gWVPMtx);
}