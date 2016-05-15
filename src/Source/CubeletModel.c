/*
Copyright 2016 Sean Paget
Email: codeMonkey760@hotmail.com
    
This file is part of gCube.

gCube is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gCube is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with gCube.  If not, see <http://www.gnu.org/licenses/>.
Please read COPYING.txt for details
*/

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "CubeletModel.h"

#define FIXED_IMAGES_BINARY_SIZE 127122

extern char _binary_src_Resources_images_bin_start[];
extern char _binary_src_Resources_images_bin_end[];
unsigned int imagesBlobSize = 0;

static GLuint vbos[7] = {-1};
static GLuint textures[6] = {-1};
static char textureNames[6][64] = {0};

static float diffuseColors[7][3] = {0};

char *cubeletWFO;
char *cubeletMTL;

void InitCubeletVBOs (void) {
    _ParseWFO();
    _ParseMTL();
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

void InitTextures (void) {
    int i;
    imagesBlobSize = _binary_src_Resources_images_bin_end - _binary_src_Resources_images_bin_start;
    if (
        _binary_src_Resources_images_bin_start == NULL ||
        _binary_src_Resources_images_bin_end == NULL ||
        imagesBlobSize != FIXED_IMAGES_BINARY_SIZE
    ) {
        fprintf(stderr, "A serious linking error has occured. Please ensure images.o was properly linked.");
        exit(1);
    }
    
    char *curPos = _binary_src_Resources_images_bin_start;
    if (_CheckTextureHeader(&curPos) != true) {
        fprintf(stderr, "Texture blob header does not match. Please ensure that images.o was linked correctly.");
        exit(1);
    }
    
    glGenTextures(6,textures);
    for (i = 0; i < 6; ++i) {
        _InitTextureFromBlob(i,textures[i],&curPos);
    }
}

void DestroyTextures (void) {
    int i;
    glDeleteTextures(6, textures);
    for (i = 0; i < 6; ++i) {
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
    
    return length == 6;
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
    if (index < 0 || index > 6) return -1;
    
    return textures[index];
}

int GetTextureByName(char *name) {
    int index = -1;
    int i;
    
    for (i = 0; i < 6; ++i) {
        if (strcmp(name,textureNames[i]) == 0) {
            return GetTextureByIndex(i);
        }
    }
}

void GetDiffuseColor(sticker sticker_id, float dst[3]) {
    int i;
    if (sticker_id < 0 || sticker_id > 6 || dst == NULL) return;
    
    for (i = 0; i < 3; ++i) {
        dst[i] = diffuseColors[sticker_id][i];
    }
}

void _ParseWFO (void) {
    int wfoLen = strlen(cubeletWFO);
    int i;
    int numV = 0, numVN = 0, numTC = 0;
    char *curPos = cubeletWFO;
    int faceCounts[7] = {0};
    int curFace = -1; //expecting usemtl tag before any writes to faceCounts
    
    // PHASE 1:
    // count vertex lines
    // count vertex normal lines
    // count tex coord lines
    // count faces for each buffer
    // NOTE: expecting seven buffers
    while (curPos < cubeletWFO + wfoLen) {
        if (_CompareTags(curPos,"v")) {
            numV++;
        } else if (_CompareTags(curPos, "vn")) {
            numVN++;
        } else if (_CompareTags(curPos, "vt")) {
            numTC++;
        } else if (_CompareTags(curPos, "f")) {
            faceCounts[curFace]++;
        } else if (_CompareTags(curPos, "usemtl")) {
            curFace++;
        }
        curPos = _NextLine(curPos);
    }
    
    // PHASE 2:
    // Grab memory
    float *vArray = NULL, *vArrayPos = NULL;
    float *vnArray = NULL, *vnArrayPos = NULL;
    float *tcArray = NULL, *tcArrayPos = NULL;
    float *vboArrayPos = NULL;
    float *vboArray[7] = {NULL};
    curFace = -1;
    
    vArray = vArrayPos = calloc(numV,sizeof(float) * 3);
    vnArray = vnArrayPos = calloc(numVN, sizeof(float) * 3);
    tcArray = tcArrayPos = calloc(numTC, sizeof(float) * 2);
    for(i = 0; i < 7; ++i) {
        vboArray[i] = calloc(faceCounts[i], sizeof(float) * 8 * 3);
    }
    
    // Parse data into memory
    curPos = cubeletWFO;
    while (curPos < cubeletWFO + wfoLen) {
        if (_CompareTags(curPos,"v")) {
            _ParseFloatArray(curPos,&vArrayPos,3,false);
        } else if (_CompareTags(curPos,"vn")) {
            _ParseFloatArray(curPos,&vnArrayPos,3,false);
        } else if (_CompareTags(curPos,"vt")) {
            _ParseFloatArray(curPos,&tcArrayPos,2,false);
        } else if (_CompareTags(curPos,"f")) {
            _CopyFullVertex(curPos,&vboArrayPos,vArray,vnArray,tcArray,true);
        } else if (_CompareTags(curPos,"usemtl")) {
            vboArrayPos = vboArray[++curFace];
        }
        curPos = _NextLine(curPos);
    }
    
    // PHASE 3:
    // Generate VBO's with fresh new data
    
    glGenBuffers(7,vbos);
    for (i = 0; i < 7; ++i) {
        glBindBuffer(GL_ARRAY_BUFFER,vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, faceCounts[i] * sizeof(float) * 8 * 3, vboArray[i], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
    
    free(vArray); vArray = vArrayPos = NULL;
    free(vnArray); vnArray = vnArrayPos = NULL;
    free(tcArray); tcArray = tcArrayPos = NULL;
    for (i = 0; i < 7; ++i) {
        free(vboArray[i]); vboArray[i] = NULL;
    }
    vboArrayPos = NULL;
}

void _ParseFloatArray (char *curPos, float **arrayPos, int numFloats, bool swapyz) {
    char target[10] = {0};
    int copyIndex = 0;
    int i;
    float temp = 0.0f;
    float buffer[3] = {0.0f};
    
    if (
        curPos      == NULL || 
        arrayPos    == NULL || 
        (*arrayPos) == NULL || 
        numFloats    < 1    || 
        numFloats    > 3
    ) {
        return;
    }
    
    // advance past the tag in this line
    curPos = _NextFloat(curPos);
    
    // for the num of floats to pull out of this line
    for (i = 0; i < numFloats; ++i) {
        
        // copy chars up to a space onto the stack
        while (*curPos != 0 && *curPos != '\n' && *curPos != ' ') {
            target[copyIndex++] = *(curPos++);
        }
        curPos++;
    
        // convert from ascii and copy to buffer
        buffer[i] = (float) atof(target);
        
        // reset some locals
        memset(target,0,sizeof(char) * 10);
        copyIndex = 0;
    }
    
    // if swap yz is true then swap the last two elements in the buffer
    if (swapyz == true) {
        temp = buffer[0];
        buffer[0] = buffer[numFloats-1];
        buffer[numFloats-1] = temp;
    }
    
    //copy the buffer into the target
    for (i = 0; i < numFloats; ++i) {
        (*arrayPos)[i] = buffer[i];
    }
    
    //advance the supplied pointer
    (*arrayPos) += numFloats;
}

void _CopyFullVertex (char *curPos, float **arrayPos, float *vArray, float *vnArray, float *tcArray, bool swapVertex) {
    float copyOfPoly[24] = {0.0f};
    float *curFloat = copyOfPoly;
    float temp = 0.0f;
    int indices[3] = {0};
    int currentIndex = 0;
    char curNum[4] = {0};
    char *curChar = NULL;
    int i;
    
    if (
        curPos      == NULL || 
        arrayPos    == NULL || 
        (*arrayPos) == NULL || 
        vArray      == NULL || 
        vnArray     == NULL || 
        tcArray     == NULL
    ) {
        return;
    }
    
    // remember: working on a line that looks like this:
    // f 32/1/11 38/1/11 37/1/11\n
    
    // advance past the f tag
    curPos = _NextFloat (curPos);
    
    // for every vertex in this polygon
    for (i = 0; i < 3; ++i) {
    
        // convert slash separated indices into ints
        // for every index in the current vertex
        for (currentIndex = 0; currentIndex < 3; ++currentIndex) {
            curChar = curNum;
            while (*curPos != '/' && *curPos != ' ' && *curPos != '\n') {
                *(curChar++) = *(curPos++);
            }
            curPos++;
            curChar++;

            // remember that indices from wfo are 1 based not zero so subtract 1
            indices[currentIndex] = atoi(curNum) - 1;
            memset(curNum,0,sizeof(char) * 4);
            curChar = curNum;
        }

        // use indices to copy data from supporting buffers
        curFloat[0] = vArray[(indices[0] * 3) + 0];
        curFloat[1] = vArray[(indices[0] * 3) + 1];
        curFloat[2] = vArray[(indices[0] * 3) + 2];

        curFloat[3] = vnArray[(indices[2] * 3) + 0];
        curFloat[4] = vnArray[(indices[2] * 3) + 1];
        curFloat[5] = vnArray[(indices[2] * 3) + 2];

        curFloat[6] = tcArray[(indices[1] * 2) + 0];
        curFloat[7] = tcArray[(indices[1] * 2) + 1];

        curFloat += 8;
    }
    
    // if swapping the vertices to fix winding order
    if (swapVertex == true) {
        for (i = 0; i < 8; ++i) {
            temp = copyOfPoly[i];
            copyOfPoly[i] = copyOfPoly[i+16];
            copyOfPoly[i+16] = temp;
        }
    }
    
    // copy the results over to the supplied vbo and increment it's pointer
    for (i = 0; i < 24; ++i) {
        (*arrayPos)[i] = copyOfPoly[i];
    }
    (*arrayPos) += 24;
}

char *_NextFloat (char *curPos) {
    if (curPos == NULL) return NULL;
    
    while ( *curPos != 0 && *curPos != '\n' && *curPos != ' ') {
        ++curPos;
    }
    
    if (*curPos != 0) curPos++;
    
    return curPos;
}

void _ParseMTL (void) {
    char *curPos = cubeletMTL;
    int len = strlen(curPos);
    int curMtl = -1;
    float stackColors[21] = {0.0f};
    float *curFloat = stackColors;
    int i;
    
    int indexArray[7] = {2,1,5,6,4,0,3};
    int curIndex = 0;
    
    while (curPos < (cubeletMTL + len)) {
        if (_CompareTags("newmtl",curPos) == true) {
            curMtl++;
        } else if(_CompareTags("Kd",curPos) == true) {
            _ParseFloatArray(curPos, &curFloat, 3, false);
        }
        curPos = _NextLine(curPos);
    }
    
    for (i = 0; i < 7; ++i) {
        curIndex = indexArray[i];
        diffuseColors[curIndex][0] = stackColors[i*3+0];
        diffuseColors[curIndex][1] = stackColors[i*3+1];
        diffuseColors[curIndex][2] = stackColors[i*3+2];
    }
}

bool _CompareTags (char *tag1, char *tag2) {
    if (tag1 == NULL || tag2 == NULL) return false;
    
    while (
        (*tag1) != 0 &&
        (*tag1) != ' ' &&
        (*tag2) != 0 &&
        (*tag2) != ' '
    ) {
        if ( (*tag1) != (*tag2) ) return false;
        tag1++;
        tag2++;
    }
    
    if ( 
        ((*tag1) == 0 || (*tag1) == ' ') &&
        ((*tag2) == 0 || (*tag2) == ' ')
    ) {
        return true;
    } else {
        return false;
    }
}

char* _NextLine (char *curPos) {
    if (curPos == NULL) return NULL;
    
    while (*curPos != 0 && *curPos != '\n') {
        curPos++;
    }
    
    if (*curPos == '\n') {
        return curPos + 1;
    } else {
        return curPos;
    }
}

char *cubeletWFO = 
"# Blender v2.74 (sub 0) OBJ File: 'cubelet.blend'\n"
"# www.blender.org\n"
"mtllib cubelet.mtl\n"
"o Cube\n"
"v 0.906828 -0.989145 -0.859575\n"
"v 0.906828 -0.989145 0.859575\n"
"v -0.906828 -0.989145 0.859575\n"
"v -0.906827 -0.989145 -0.859576\n"
"v 0.906828 0.989145 -0.859575\n"
"v -0.906828 0.989145 -0.859575\n"
"v -0.906828 0.989145 0.859575\n"
"v 0.906827 0.989145 0.859576\n"
"v 0.989145 -0.906828 -0.859575\n"
"v 0.989146 0.906828 -0.859575\n"
"v 0.989145 0.906828 0.859576\n"
"v 0.989145 -0.906828 0.859575\n"
"v 0.906828 -0.906828 0.989145\n"
"v 0.906827 0.906828 0.989146\n"
"v -0.906828 0.906828 0.989145\n"
"v -0.906828 -0.906828 0.989145\n"
"v -0.989145 -0.906828 0.859575\n"
"v -0.989146 0.906828 0.859575\n"
"v -0.989145 0.906828 -0.859575\n"
"v -0.989145 -0.906828 -0.859576\n"
"v 0.906828 0.906828 -0.989145\n"
"v 0.906828 -0.906828 -0.989145\n"
"v -0.906827 -0.906828 -0.989146\n"
"v -0.906828 0.906828 -0.989145\n"
"v 1.000000 -1.000000 -1.000000\n"
"v 1.000000 -1.000000 1.000000\n"
"v -1.000000 -1.000000 1.000000\n"
"v -1.000000 -1.000000 -1.000000\n"
"v 1.000000 1.000000 -0.999999\n"
"v 0.999999 1.000000 1.000001\n"
"v -1.000000 1.000000 1.000000\n"
"v -1.000000 1.000000 -1.000000\n"
"v 0.828479 -0.953362 -0.828479\n"
"v 0.828479 -0.953362 0.828479\n"
"v -0.828479 -0.953362 0.828479\n"
"v -0.828479 -0.953362 -0.828480\n"
"v 0.828480 0.953362 -0.828479\n"
"v -0.828479 0.953362 -0.828479\n"
"v -0.828480 0.953362 0.828479\n"
"v 0.828479 0.953362 0.828480\n"
"v 0.953362 -0.828479 -0.828479\n"
"v 0.953363 0.828479 -0.828479\n"
"v 0.953361 0.828479 0.828480\n"
"v 0.953362 -0.828479 0.828479\n"
"v 0.828479 -0.828479 0.953362\n"
"v 0.828479 0.828479 0.953363\n"
"v -0.828480 0.828479 0.953362\n"
"v -0.828479 -0.828479 0.953362\n"
"v -0.953362 -0.828479 0.828479\n"
"v -0.953362 0.828479 0.828479\n"
"v -0.953362 0.828479 -0.828479\n"
"v -0.953362 -0.828479 -0.828480\n"
"v 0.828480 0.828479 -0.953362\n"
"v 0.828479 -0.828479 -0.953362\n"
"v -0.828479 -0.828479 -0.953362\n"
"v -0.828479 0.828479 -0.953362\n"
"vt 0.000000 0.000000\n"
"vt 1.000000 1.000000\n"
"vt 0.000000 1.000000\n"
"vt 1.000000 0.000000\n"
"vn 0.000000 -1.000000 0.000000\n"
"vn 0.000000 1.000000 0.000000\n"
"vn 1.000000 0.000000 0.000000\n"
"vn -0.000000 0.000000 1.000000\n"
"vn -1.000000 -0.000000 -0.000000\n"
"vn 0.000000 0.000000 -1.000000\n"
"vn -0.262400 -0.965000 0.000000\n"
"vn 0.000000 -0.965000 -0.262400\n"
"vn 0.262400 -0.965000 0.000000\n"
"vn -0.000000 -0.965000 0.262400\n"
"vn -0.000000 0.965000 0.262400\n"
"vn 0.262400 0.965000 0.000000\n"
"vn 0.000000 0.965000 -0.262400\n"
"vn -0.262400 0.965000 -0.000000\n"
"vn 0.965000 -0.000000 0.262400\n"
"vn 0.965000 -0.262400 0.000000\n"
"vn 0.965000 0.000000 -0.262400\n"
"vn 0.965000 0.262400 0.000000\n"
"vn -0.262400 -0.000000 0.965000\n"
"vn -0.000000 -0.262400 0.965000\n"
"vn 0.262400 0.000000 0.965000\n"
"vn 0.000000 0.262400 0.965000\n"
"vn -0.965000 -0.000000 -0.262400\n"
"vn -0.965000 -0.262400 -0.000000\n"
"vn -0.965000 -0.000000 0.262400\n"
"vn -0.965000 0.262400 -0.000000\n"
"vn -0.262400 0.000000 -0.965000\n"
"vn 0.000000 0.262400 -0.965000\n"
"vn 0.262400 0.000000 -0.965000\n"
"vn 0.000000 -0.262400 -0.965000\n"
"usemtl Frame\n"
"s off\n"
"f 33/1/1 34/1/1 35/1/1\n"
"f 38/1/2 39/1/2 40/1/2\n"
"f 42/1/3 43/1/3 44/1/3\n"
"f 46/1/4 47/1/4 48/1/4\n"
"f 49/1/5 50/1/5 51/1/5\n"
"f 54/1/6 55/1/6 56/1/6\n"
"f 26/1/7 34/1/7 33/1/7\n"
"f 27/1/8 35/1/8 34/1/8\n"
"f 28/1/9 36/1/9 35/1/9\n"
"f 25/1/10 33/1/10 36/1/10\n"
"f 32/1/11 38/1/11 37/1/11\n"
"f 31/1/12 39/1/12 38/1/12\n"
"f 30/1/13 40/1/13 39/1/13\n"
"f 29/1/14 37/1/14 40/1/14\n"
"f 29/1/15 42/1/15 41/1/15\n"
"f 30/1/16 43/1/16 42/1/16\n"
"f 30/1/17 26/1/17 44/1/17\n"
"f 26/1/18 25/1/18 41/1/18\n"
"f 30/1/19 46/1/19 45/1/19\n"
"f 30/1/20 31/1/20 47/1/20\n"
"f 31/1/21 27/1/21 48/1/21\n"
"f 26/1/22 45/1/22 48/1/22\n"
"f 31/1/23 50/1/23 49/1/23\n"
"f 32/1/24 51/1/24 50/1/24\n"
"f 28/1/25 52/1/25 51/1/25\n"
"f 27/1/26 49/1/26 52/1/26\n"
"f 25/1/27 54/1/27 53/1/27\n"
"f 28/1/28 55/1/28 54/1/28\n"
"f 28/1/29 32/1/29 56/1/29\n"
"f 32/1/30 29/1/30 53/1/30\n"
"f 36/1/1 33/1/1 35/1/1\n"
"f 37/1/2 38/1/2 40/1/2\n"
"f 41/1/3 42/1/3 44/1/3\n"
"f 45/1/4 46/1/4 48/1/4\n"
"f 52/1/5 49/1/5 51/1/5\n"
"f 53/1/6 54/1/6 56/1/6\n"
"f 25/1/7 26/1/7 33/1/7\n"
"f 26/1/8 27/1/8 34/1/8\n"
"f 27/1/9 28/1/9 35/1/9\n"
"f 28/1/10 25/1/10 36/1/10\n"
"f 29/1/11 32/1/11 37/1/11\n"
"f 32/1/12 31/1/12 38/1/12\n"
"f 31/1/13 30/1/13 39/1/13\n"
"f 30/1/14 29/1/14 40/1/14\n"
"f 25/1/15 29/1/15 41/1/15\n"
"f 29/1/16 30/1/16 42/1/16\n"
"f 43/1/17 30/1/17 44/1/17\n"
"f 44/1/18 26/1/18 41/1/18\n"
"f 26/1/19 30/1/19 45/1/19\n"
"f 46/1/20 30/1/20 47/1/20\n"
"f 47/1/21 31/1/21 48/1/21\n"
"f 27/1/22 26/1/22 48/1/22\n"
"f 27/1/23 31/1/23 49/1/23\n"
"f 31/1/24 32/1/24 50/1/24\n"
"f 32/1/25 28/1/25 51/1/25\n"
"f 28/1/26 27/1/26 52/1/26\n"
"f 29/1/27 25/1/27 53/1/27\n"
"f 25/1/28 28/1/28 54/1/28\n"
"f 55/1/29 28/1/29 56/1/29\n"
"f 56/1/30 32/1/30 53/1/30\n"
"usemtl PosX\n"
"f 10/2/3 11/3/3 12/1/3\n"
"f 9/4/3 10/2/3 12/1/3\n"
"usemtl NegX\n"
"f 18/2/5 19/3/5 20/1/5\n"
"f 17/4/5 18/2/5 20/1/5\n"
"usemtl PosY\n"
"f 22/1/6 23/4/6 24/2/6\n"
"f 21/3/6 22/1/6 24/2/6\n"
"usemtl NegY\n"
"f 14/2/4 15/3/4 16/1/4\n"
"f 13/4/4 14/2/4 16/1/4\n"
"usemtl PosZ\n"
"f 6/3/2 7/1/2 8/4/2\n"
"f 5/2/2 6/3/2 8/4/2\n"
"usemtl NegZ\n"
"f 2/2/1 3/3/1 4/1/1\n"
"f 1/4/1 2/2/1 4/1/1\n";

char *cubeletMTL = 
"# Blender MTL File: 'cubelet.blend'\n"
"# Material Count: 7\n"

"newmtl Frame\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 1.000000 1.000000 1.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd Frame.png\n"

"newmtl PosX\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 0.000000 0.000000 1.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd blender.png\n"

"newmtl NegX\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 0.000000 1.000000 0.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd blender.png\n"

"newmtl PosY\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 1.000000 1.000000 1.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd blender.png\n"

"newmtl NegY\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 1.000000 1.000000 0.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd blender.png\n"

"newmtl PosZ\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 1.000000 0.000000 0.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd blender.png\n"

"newmtl NegZ\n"
"Ns 96.078431\n"
"Ka 0.000000 0.000000 0.000000\n"
"Kd 1.000000 0.350000 0.000000\n"
"Ks 0.000000 0.000000 0.000000\n"
"Ni 1.000000\n"
"d 1.000000\n"
"illum 0\n"
"map_Kd blender.png\n";
