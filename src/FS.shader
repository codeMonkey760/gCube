#version 440

in vec3 posW;
in vec3 normW;
in vec2 outtexC;

uniform vec3 gDiffuseColor;
uniform vec3 gCamPos;

void main (void) {
    vec3 toCam = normalize(posW - gCamPos);
    float diffuse = max(dot(toCam,normW),0.0f);

    gl_FragColor = vec4(gDiffuseColor * diffuse,1.0f);
}