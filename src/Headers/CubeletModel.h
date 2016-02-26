#ifndef _C_RUBE_CUBELETMODEL_H
#define _C_RUBE_CUBELETMODEL_H

void InitCubeletVBOs (void);
void DestroyCubeletVBOs (void);
int GetCubeletVBO (int index);
void _InitVBOFromBlob (int vbo, char **curPos);
bool _CheckBlobHeader (char **curPos);

void InitTextures (void);
void DestroyTextures (void);
bool _CheckTextureHeader (char **curPos);
void _InitTextureFromBlob (int index, int buffer, char **curPos);
int GetTextureByIndex(int index);
int GetTextureByName (char *name);

#endif