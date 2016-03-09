#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Camera.h"
#include "Util.h"
#include "Camera.h"
#include "Cubelet.h"
#include "Cube.h"
#include "GUIButton.h"
#include "GUIRenderer.h"
#include "GUIShader.h"

GUIRenderer guiRenderer;

void InitGUIRenderer (void) {
    memset(&guiRenderer, 0, sizeof(GUIRenderer));
    
    _BuildVBO();
}

void DrawButtons (GUIButton *buttons, int numButtons) {
    int i;
    GUIButton *cb = NULL;
    if (buttons == NULL || numButtons < 1 || guiShader.shader == -1) return;
    
    glUseProgram(guiShader.shader);
    glBindBuffer(GL_ARRAY_BUFFER,guiRenderer.buttonVBO);
    
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GEQUAL, 0.5f);
    
    for (i = 0; i < numButtons; ++i) {
        cb = &(buttons[i]);
        if (
            cb == NULL ||
            cb->visible == false ||
            cb->texId == -1
        ) continue;
        
        glUniformMatrix4fv(guiShader.gTMtx,1,GL_TRUE,cb->tMtx);
        glUniformMatrix3fv(guiShader.gTexMtx, 1, GL_TRUE, cb->texMtx);
        glUniform1i(guiShader.gTexture, 0);
        glUniform3fv(guiShader.gAmbient,1,cb->ambientColor);
        glUniform1f(guiShader.gMixValue, cb->mixValue);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cb->texId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glEnableVertexAttribArray(guiShader.inPosL);
        glEnableVertexAttribArray(guiShader.inTexC);
        
        glVertexAttribPointer(guiShader.inPosL,3,GL_FLOAT,GL_FALSE, 20, 0);
        glVertexAttribPointer(guiShader.inTexC,2,GL_FLOAT,GL_FALSE, 20, 12);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(guiShader.inPosL);
        glDisableVertexAttribArray(guiShader.inTexC);
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
    
    glDisable(GL_ALPHA_TEST);
}

void DestroyGUIRenderer (void) {
    glDeleteBuffers(1, &guiRenderer.buttonVBO);
    guiRenderer.buttonVBO = -1;
}

void _BuildVBO (void) {
    float vb[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
        
         1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    
    glGenBuffers(1,&guiRenderer.buttonVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER,guiRenderer.buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 5 * 4, vb, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}