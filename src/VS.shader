in vec3 posL;

uniform mat4 gWVPMtx;

void main (void) {
	gl_Position = mul(vec4(posL,1.0f),gWVPMtx);
}