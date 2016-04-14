#ifndef _C_RUBE_CUBELETMODEL_H
#define _C_RUBE_CUBELETMODEL_H

/*
 Note to self ...
 * with the object file built in the way it is ...
 * it looks like index vbo index 0 will be the frame and
 * vbo 1 will be pos x
 * vbo 2 will be neg x
 * vbo 3 will be neg z
 * vbo 4 will be pos z
 * vbo 5 will be pos y
 * vbo 6 will be neg y
 * make an enum for this
 */

typedef enum {
    STICKER_POS_X, 
    STICKER_NEG_X, 
    STICKER_NEG_Z, 
    STICKER_POS_Z, 
    STICKER_POS_Y, 
    STICKER_NEG_Y
} sticker;

void InitCubeletVBOs (void);
void DestroyCubeletVBOs (void);
int GetCubeletVBO (int index);

void InitTextures (void);
void DestroyTextures (void);
bool _CheckTextureHeader (char **curPos);
void _InitTextureFromBlob (int index, int buffer, char **curPos);
int GetTextureByIndex(int index);
int GetTextureByName (char *name);

void GetDiffuseColor(sticker sticker_id, float dst[3]);

void _ParseWFO (void);
bool _CompareTags (char *tag1, char *tag2);
char* _NextLine (char *block);
void _ParseFloatArray(char *curPos, float **arrayPos, int numFloats, bool swapyz);
void _CopyFullVertex(char *curPos, float **arrayPos, float *vArray, float *vnArray, float *tcArray, bool swapVerts);
char *_NextFloat (char *curPos);

void _ParseMTL (void);

#endif