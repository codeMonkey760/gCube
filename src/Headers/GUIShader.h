#ifndef _C_RUBE_GUISHADER_H_
#define _C_RUBE_GUISHADER_H_

typedef struct {
    int shader;
    int vs;
    int fs;
    
    char *vsSrcName;
    char *fsSrcName;
    
    int inPosL;
    int inTexC;
    
    int gTMtx;
    int gTexMtx;
    
    int gTexture;
    int gAmbient;
    int gMixValue;
} GUIShader;

extern GUIShader guiShader;

bool InitGUIShader (void);
void DestroyGUIShader (void);

#endif