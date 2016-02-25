#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "CubeletModel.h"

#define FIXED_CUBELET_BINARY_SIZE 7153

extern char _binary_CubeletRes_bin_start[];
extern char _binary_CubeletRes_bin_end[];
unsigned int cubeletBlobSize = 0;

static GLuint vbos[7] = {-1};

void InitCubeletVBOs (void) {
    cubeletBlobSize = _binary_CubeletRes_bin_end - _binary_CubeletRes_bin_start;
    if (
        _binary_CubeletRes_bin_start == NULL ||
        _binary_CubeletRes_bin_end   == NULL ||
        cubeletBlobSize != FIXED_CUBELET_BINARY_SIZE
    ) {
        fprintf(stderr, "A Serious linking error has occured. Ensure that cubelet.o has been linked correctly!\n");
        exit(1);
    }
    
    char *curPos = _binary_CubeletRes_bin_start;
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
    int i;
    glDeleteBuffers(7,vbos);
    for (i = 0; i < 7; ++i) {
        vbos[i] = -1;
    }
}

int GetCubeletVBO (int index) {
    if (index < 0 || index > 6) return -1;
    
    return vbos[index];
}

void _InitVBOFromBlob (int vbo, char **curPos) {
    int i;
    float *fCurPos = NULL;
    if (curPos == NULL || (*curPos) == NULL || vbo == -1)  return;

    int length = *( (int*) (*curPos));
    (*curPos) += 4;
    length = length / 2;
    
    /*
     * Test Code ....
     * I thought there might be corruption
     * so I printed this structure 
    fCurPos = (float*) (*curPos);
    printf("Block length: %d\n", length);
    for (i = 0; i < length / 4; i = i + 8) {
        printf(
            "pos: %.2f %.2f %.2f norm: %.2f %.2f %.2f tex: %.2f %.2f\n",
            fCurPos[i+0],
            fCurPos[i+1],
            fCurPos[i+2],

            fCurPos[i+3],
            fCurPos[i+4],
            fCurPos[i+5],

            fCurPos[i+6],
            fCurPos[i+7]
        );
    }
    printf("\n");
    */
    
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, length, (float*) (*curPos), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    (*curPos) += length;
    
    return;
}

bool _CheckBlobHeader (char **curPos) {
    int length = -1;
    if (curPos == NULL || (*curPos) == NULL) return false;
    
    if (
        (*curPos)[0] != 'S' ||
        (*curPos)[1] != 'P' ||
        (*curPos)[2] !=  0  ||
        (*curPos)[3] !=  1  ||
        (*curPos)[4] != 'o' ||
        (*curPos)[5] != 'b' ||
        (*curPos)[6] != 'j'
    ) return false;
    
    (*curPos) += 7;
    
    length = *((int*) (*curPos));
    if (length != 7) return false;
    
    (*curPos) += 4;
    
    return true;
}