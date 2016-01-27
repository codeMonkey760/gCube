#ifndef _C_RUBE_SHADER_H_
#define _C_RUBE_SHADER_H_

extern int shader;
extern int vs;
extern int fs;

extern char *vsSrcName;
extern char *fsSrcName;

extern int inPosL;
extern int inNormL;
extern int inTexC;

extern int gWMtx;
extern int gWITMtx;
extern int gWVPMtx;
extern int gDiffuseColor;
extern int gCamPos;

#define INVALID_SHADER_ID -1

int InitShader (void);
void DestroyShader (void);

int _CompileShader (GLenum type, char *src);

#endif
