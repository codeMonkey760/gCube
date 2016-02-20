#ifndef _C_RUBE_CUBELETMODEL_H
#define _C_RUBE_CUBELETMODEL_H

void InitCubeletVBOs (void);
void DestroyCubeletVBOs (void);
int GetCubeletVBO (int index);
int _InitVBOFromBlob (char **curPos);
bool _CheckBlobHeader (char **curPos);

#endif