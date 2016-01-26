#ifndef _C_RUBE_CUBE_MODEL_H_
#define _C_RUBE_CUBE_MODEL_H_

void InitCubeVBO (void);
void DestroyCubeVBO (void);
int GetCubeVBO (void);

static const GLfloat cube[] = {
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

	 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
};

#define VBO_SIZE_IN_FLOATS 18
#define VBO_SIZE_IN_BYTES  72
#define VBO_SIZE_IN_POLYS  2
#define VBO_SIZE_IN_INDICES 6

#endif
