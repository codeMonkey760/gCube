#version 440

in vec3 posW;
in vec3 normW;
in vec2 outtexC;

uniform vec3 gDiffuseColor;
uniform vec3 gCamPos;

void main (void) {
    vec3 toCam = normalize(posW - gCamPos);
    vec3 normW2 = normalize(normW);
    //float diffuse = max(dot(toCam,normW2),0.0f);
    float diffuse = min(dot(toCam,normW2), 0.0f);
    diffuse *= -1.0f;

    gl_FragColor = vec4((gDiffuseColor * diffuse),1.0f);
    //gl_FragColor = vec4((gDiffuseColor * (1.0f - (diffuse * .01f))),1.0f);
}