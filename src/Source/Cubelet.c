#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Camera.h"
#include "Util.h"
#include "Cubelet.h"
#include "Shader.h"
#include "CubeletModel.h"

void InitCubeletArray(Cubelet *array, int numCubelets){
    int i;
    Cubelet *curCubelet = NULL;
    if (array == NULL || numCubelets < 1) return;
    
    memset(array,0,sizeof(Cubelet) * numCubelets);
    
    for (i = 0; i < numCubelets; ++i) {
        curCubelet = &array[i];
        
        // init to 100% scale
        curCubelet->scale[0] = 1.0f;
        curCubelet->scale[1] = 1.0f;
        curCubelet->scale[2] = 1.0f;
        
        // identity quaternion
        curCubelet->rotation[3] = 1.0f;
    }
}

void DrawCubeletArray (Cubelet *array, int numCubelets, Camera *cam) {
    int i, j, texture, sticker;
    Cubelet *curCubelet = NULL;
    float
        wMtx[16] = {0.0f},
        witMtx[16] = {0.0f},
        wvpMtx[16] = {0.0f},
        texMtx[9] = {0.0f},
        vpMtx[16] = {0.0f},
        dColor[4] = {1.0f,1.0f,1.0f,1.0f}
    ;
    
    if (
        array == NULL ||
        numCubelets < 1 ||
        shader == INVALID_SHADER_ID ||
        cam == NULL
    ) return;
    
    Mat4Mult(vpMtx,cam->viewMtx,cam->projMtx);
    texMtx[0] = 1.0f;
    texMtx[4] = -1.0f;
    texMtx[8] = 1.0f;
    
    texture = GetTextureByName("arrow.png");
    sticker = GetTextureByName("blender.png");
    
    glUseProgram(shader);
    
    for (i = 0; i < numCubelets; ++i) {
        curCubelet = &array[i];
        BuildMatricies(curCubelet, wMtx, witMtx);
        Mat4Mult(wvpMtx, wMtx, vpMtx); 
        
        glUniform3fv(gCamPos, 1, cam->camPosW);
        glUniformMatrix4fv(gWMtx,1,GL_TRUE,wMtx);
        glUniformMatrix4fv(gWITMtx, 1, GL_TRUE, witMtx);
        glUniformMatrix4fv(gWVPMtx, 1, GL_TRUE, wvpMtx);
        glUniformMatrix3fv(gTexMtx, 1, GL_TRUE, texMtx);
        GetStickerColor(7,dColor);
        glUniform3fv(gDiffuseColor,1, dColor);
        
        glEnableVertexAttribArray(inPosL);
        glEnableVertexAttribArray(inNormL);
        glEnableVertexAttribArray(inTexC);
        
        // render cube frame
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(gTexture,0);
        glBindBuffer(GL_ARRAY_BUFFER, GetCubeletVBO(0));
        glVertexAttribPointer(inPosL, 3, GL_FLOAT, GL_FALSE, 32, 0);
        glVertexAttribPointer(inNormL, 3, GL_FLOAT, GL_FALSE, 32, 12);
        glVertexAttribPointer(inTexC, 2, GL_FLOAT, GL_FALSE, 32, 24);
        glDrawArrays(GL_TRIANGLES, 0, 180);
        
        // render any applied stickers
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sticker);
        glUniform1i(gTexture,0);
        for (j = 0; j < 6; ++j) {
            if (curCubelet->stickers[j] == true) {
                GetStickerColor(j,dColor);
                glUniform3fv(gDiffuseColor,1, dColor);
                glBindBuffer(GL_ARRAY_BUFFER, GetCubeletVBO(j+1));
                glVertexAttribPointer(inPosL, 3, GL_FLOAT, GL_FALSE, 32, 0);
                glVertexAttribPointer(inNormL, 3, GL_FLOAT, GL_FALSE, 32, 12);
                glVertexAttribPointer(inTexC, 2, GL_FLOAT, GL_FALSE, 32, 24);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glDisableVertexAttribArray(inPosL);
        glDisableVertexAttribArray(inNormL);
        glDisableVertexAttribArray(inTexC);
    }
    
    glUseProgram(0);
    
}

void BuildMatricies (Cubelet *singleCubelet, float wMtx[16], float witMtx[16]) {
    if (singleCubelet == NULL || wMtx == NULL || witMtx == NULL) return;
    
    float tMtx[16] = {0.0f};
    float rMtx[16] = {0.0f};
    float sMtx[16] = {0.0f};
    
    Mat4Scaling(sMtx, singleCubelet->scale);
    Mat4RotationQuaternion(rMtx, singleCubelet->rotation);
    Mat4Translation(tMtx, singleCubelet->posW);
    
    Mat4Mult(wMtx, sMtx, rMtx);
    Mat4Mult(wMtx, wMtx, tMtx);
    
    Mat4Inverse(witMtx, wMtx);
}