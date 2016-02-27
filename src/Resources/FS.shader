#version 440

in vec3 posW;
in vec3 normW;
in vec2 outtexC;

uniform vec3 gDiffuseColor;
uniform vec3 gCamPos;

uniform sampler2D gTexture;

void main (void) {
    vec3 toCam = normalize(posW - gCamPos);
    vec3 normW2 = normalize(normW);

    // this diffuse calculation is a little odd...
    // remember that we're lighting back faces not front faces
    // so the dot products come out neg
    float diffuse = min(dot(toCam,normW2), 0.0f);
    diffuse = (diffuse * -0.7f) + 0.3f;
    
    vec3 col = texture(gTexture, outtexC).rgb * gDiffuseColor;

    gl_FragColor = vec4((col * diffuse),1.0f);
}