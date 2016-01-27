#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "QuadModel.h"

static GLuint vbo = -1;

void InitCubeVBO (void) {
	glGenBuffers(1,&vbo);

	glBindBuffer(GL_ARRAY_BUFFER,vbo);

	glBufferData(GL_ARRAY_BUFFER, VBO_SIZE_IN_BYTES, cube, GL_STATIC_DRAW);

	if (vbo == -1) {
		fprintf(stderr, "glGenBuffers failed\n");
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DestroyCubeVBO (void) {
	glDeleteBuffers(1,&vbo);
	vbo = -1;
}

int GetCubeVBO (void) {
	return vbo;
}
