#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "Shader.h"
#include "GUIShader.h"

GUIShader guiShader;

void InitGUIShader (void) {
    memset(&guiShader, 0, sizeof(GUIShader));
    
    guiShader.shader = INVALID_SHADER_ID;
    guiShader.vs = INVALID_SHADER_ID;
    guiShader.fs = INVALID_SHADER_ID;
    guiShader.vsSrcName = "src/Resources/GUIVS.shader";
    guiShader.fsSrcName = "src/Resources/GUIFS.shader";
    guiShader.inPosL = INVALID_SHADER_ID;
    guiShader.inTexC = INVALID_SHADER_ID;
    guiShader.gTMtx = INVALID_SHADER_ID;
    guiShader.gTexMtx = INVALID_SHADER_ID;
    guiShader.gTexture = INVALID_SHADER_ID;
    guiShader.gAmbient = INVALID_SHADER_ID;
    guiShader.gMixValue = INVALID_SHADER_ID;
}

void DestroyGUIShader (void) {
    ;
}