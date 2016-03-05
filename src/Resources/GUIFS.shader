#version 440

in vec2 outTexC;

uniform sampler2D gTexture;
uniform vec3 gAmbient;
uniform float gMixValue;

void main (void) {
    vec4 tex = texture2D(gTexture, outTexC).rgba;
    float a = tex.a;

    vec3 col = tex.rgb;
    col = (gMixValue * col) + (gAmbient * (1.0f - gMixValue));

    gl_FragColor = vec4(col,a);
}