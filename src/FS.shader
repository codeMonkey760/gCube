uniform vec3 gDiffuseColor;

void main (void) {
	gl_FragColor = vec4(gDiffuseColor,1.0f);
}