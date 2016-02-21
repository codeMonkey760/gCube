#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "CubeletModel.h"

#define FIXED_CUBELET_BINARY_SIZE 0

extern char * _binary_cubelet_bin_start;
extern char * _binary_cubelet_bin_end;
unsigned int cubeletBlobSize = 0;

static GLuint vbos[7] = {-1};

void InitCubeletVBOs (void) {
    if (
        _binary_cubelet_bin_start == NULL ||
        _binary_cubelet_bin_end   == NULL ||
        _binary_cubelet_bin_end - _binary_cubelet_bin_start != FIXED_CUBELET_BINARY_SIZE
    ) {
        fprintf(stderr, "A Serious linking error has occured. Ensure that cubelet.o has been linked correctly!\n");
        exit(1);
    }
    
    char *curPos = _binary_cubelet_bin_start;
    int i = 0;
    
    if (_CheckBlobHeader(&curPos) != true) {
        fprintf(stderr, "Cubelet blob header does not match.\n");
        exit(1);
    }
    
    glGenBuffers(7,vbos);
    
    for (i = 0 ; i < 7; ++i) {
        _InitVBOFromBlob(vbos[i], &curPos);
    }
}

void DestroyCubeletVBOs (void) {
    glDeleteBuffers(7,vbos);
    for (int i = 0; i < 7; ++i) {
        vbos[i] = -1;
    }
}

int GetCubeletVBO (int index) {
    if (index < 0 || index > 6) return -1;
    
    return vbos[index];
}

void _InitVBOFromBlob (int vbo, char **curPos) {
    if (curPos == NULL || (*curPos) == NULL || vbo == -1)  return;

    int length = (float) *(*curPos);
    (*curPos) += 4;
    length = length * 4;
    
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, length, (float*) (*curPos), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    (*curPos) += length;
    
    return;
}