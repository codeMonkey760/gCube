#version 440

in vec3 posL;
in vec2 intexC;

uniform mat4 gTMtx;
uniform mat3 gTexMtx;

out vec2 outTexC;

void main (void) {
    gl_Position = vec4(posL,1.0f) * gTMtx;

    outTexC = (vec3(intexC,1.0f) * gTexMtx).xy;
}