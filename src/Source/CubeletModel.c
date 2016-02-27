#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "CubeletModel.h"

#define FIXED_CUBELET_BINARY_SIZE 7153
#define FIXED_IMAGES_BINARY_SIZE 110714

extern char _binary_CubeletRes_bin_start[];
extern char _binary_CubeletRes_bin_end[];
unsigned int cubeletBlobSize = 0;

extern char _binary_images_bin_start[];
extern char _binary_images_bin_end[];
unsigned int imagesBlobSize = 0;

static GLuint vbos[7] = {-1};
static GLuint textures[5] = {-1};
static char textureNames[5][64] = {0};

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
    float *curPosF = NULL;
    if (curPos == NULL || (*curPos) == NULL || vbo == -1)  return;

    int length = *( (int*) (*curPos));
    (*curPos) += 4;
    length = length / 2;
    
    printf("Length of current block: %d bytes %d floats %d verts\n", length, length / 4, length / 32);
    curPosF = (float*) (*curPos);
    for (i = 0; i < length / 4; i+=8) {
        printf(
            "Pos: %.2f %.2f %.2f Norm: %.2f %.2f %.2f Tex: %.2f %.2f\n",
            curPosF[i+0], curPosF[i+1], curPosF[i+2],
            curPosF[i+3], curPosF[i+4], curPosF[i+5],
            curPosF[i+6], curPosF[i+7]
        );
    }
    printf("\n");
    
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

void InitTextures (void) {
    int i;
    imagesBlobSize = _binary_images_bin_end - _binary_images_bin_start;
    if (
        _binary_images_bin_start == NULL ||
        _binary_images_bin_end == NULL ||
        imagesBlobSize != FIXED_IMAGES_BINARY_SIZE
    ) {
        fprintf(stderr, "A serious linking error has occured. Please ensure images.o was properly linked.");
        exit(1);
    }
    
    char *curPos = _binary_images_bin_start;
    if (_CheckTextureHeader(&curPos) != true) {
        fprintf(stderr, "Texture blob header does not match. Please ensure that images.o was linked correctly.");
        exit(1);
    }
    
    glGenTextures(5,textures);
    for (i = 0; i < 5; ++i) {
        _InitTextureFromBlob(i,textures[i],&curPos);
    }
}

void DestroyTextures (void) {
    int i;
    glDeleteTextures(5, textures);
    for (i = 0; i < 5; ++i) {
        textures[i] = -1;
    }
}

bool _CheckTextureHeader (char **curPos) {
    int length = -1;
    if (curPos == NULL || (*curPos) == NULL) return false;
    
    if (
        (*curPos)[0] != 'S' ||
        (*curPos)[1] != 'P' ||
        (*curPos)[2] != 0   ||
        (*curPos)[3] != 1   ||
        (*curPos)[4] != 'i'   ||
        (*curPos)[5] != 'm'   ||
        (*curPos)[6] != 'g'
                
    ) return false;
    
    (*curPos) += 7;
    length = *( (int*) (*curPos));
    (*curPos) += 4;
    
    return length == 5;
}

void _InitTextureFromBlob (int index, int buffer, char **curPos) {
    if (buffer == -1 || curPos == NULL || (*curPos) == NULL) return;
    
    char imgName[64] = {0};
    int width = -1;
    int height = -1;
    unsigned int bands = 0x00000000;
    int stride = 0;
    int i = 0;
    int sizeInBytes = 0;
    GLenum type = -1;
    
    strcpy(imgName, (*curPos));
    (*curPos) += strlen(imgName) + 1;
    width = *( (int*) (*curPos));
    (*curPos) += 4;
    height = *((int*) (*curPos));
    (*curPos) += 4;
    bands = *((int*) (*curPos));
    (*curPos) += 4;
    
    if (bands == 0x00010101) {
        type = GL_RGB;
        stride = 3;
    } else if (bands == 0x01010101) {
        type = GL_RGBA;
        stride = 4;
    } else {
        fprintf(stderr, "WARNING! Unsupported image type detected! Please ensure that images.bin was linked correctly!\n");
        exit(1);
    }
    
    sizeInBytes = width * height * stride;
    glBindTexture(GL_TEXTURE_2D, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // investigate this ... never seen it before ... new feature?
    //glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, (*curPos));
    
    (*curPos) += sizeInBytes;
    
    strcpy(textureNames[index],imgName);
}

int GetTextureByIndex(int index) {
    if (index < 0 || index > 5) return -1;
    
    return textures[index];
}

int GetTextureByName(char *name) {
    int index = -1;
    int i;
    
    for (i = 0; i < 5; ++i) {
        if (strcmp(name,textureNames[i]) == 0) {
            return GetTextureByIndex(i);
        }
    }
}

void GetStickerColor(sticker sticker_id, float dst[3]) {
    if (dst == NULL) return;
    dst[0] = 0.0f;
    dst[1] = 0.0f;
    dst[2] = 0.0f;
    
    if (sticker_id == STICKER_POS_X || sticker_id == STICKER_NEG_X) {
        dst[0] = 1.0f;
    } else if (sticker_id == STICKER_POS_Y || sticker_id == STICKER_NEG_Y) {
        dst[1] = 1.0f;
    } else if (sticker_id == STICKER_POS_Z || sticker_id == STICKER_NEG_Z) {
        dst[2] = 1.0f;
    } else {
        dst[0] = 1.0f;
        dst[1] = 1.0f;
        dst[2] = 1.0f;
    }
}