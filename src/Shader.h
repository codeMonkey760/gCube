#ifndef _C_RUBE_SHADER_H_
#define _C_RUBE_SHADER_H_

extern int shader;
extern int vs;
extern int fs;

extern char *vsSrcName;
extern char *fsSrcName;

extern int inPosL;
extern int gDiffuseColor;

int InitShader (void);
void DestroyShader (void);

int _CompileShader (GLenum type, char *src);

#endif
