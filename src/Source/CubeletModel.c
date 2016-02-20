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
    
    for (i = 0 ; i < 7; ++i) {
        vbos[i] = _InitVBOFromBlob(&curPos);
    }
}

void DestroyCubeletVBOs (void) {
    ;
}

int GetCubeletVBO (int index) {
    return -1;
}

int _InitVBOFromBlob (char **curPos) {
    if (curPos == NULL || (*curPos) == NULL) return -1;
    
    
    
    return -1;
}