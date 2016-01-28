#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Util.h"
#include "Quad.h"
#include "Shader.h"
#include "Camera.h"
#include "QuadModel.h"

void InitQuadArray (Quad *array, int numQuads) {
    int i;
    Quad *curQuad = NULL;
    if (array == NULL || numQuads < 1) return;
    
    memset(array,0,sizeof(Quad) * numQuads);
    
    for (i = 0; i < numQuads; ++i) {
        curQuad = &array[i];
        
        curQuad->rightW[0] = 1.0f;
        curQuad->upW[1] = 1.0f;
        curQuad->lookW[2] = 1.0f;
        
        curQuad->scale[0] = 1.0f;
        curQuad->scale[1] = 1.0f;
        curQuad->scale[2] = 1.0f;
    }
}

void DrawQuadArray (Quad *array, int numQuads, Camera *cam) {
    int i;
    Quad *curQuad = NULL;
    float 
        wMtx[16] =   {0.0f}, 
        witMtx[16] = {0.0f}, 
        wvpMtx[16] = {0.0f},
        vpMtx[16] =  {0.0f}
    ;
    
    if (array == NULL || numQuads < 1 || shader == INVALID_SHADER_ID || cam == NULL) return;
    
    Mat4Mult(vpMtx,cam->viewMtx,cam->projMtx);
    
    glUseProgram(shader);
    
    for (i = 0; i < numQuads; ++i) {
        // possible performance suck
        Mat4Identity(wMtx);
        Mat4Identity(witMtx);
        Mat4Identity(wvpMtx);
        
        curQuad = &array[i];
        BuildMatricies(curQuad,wMtx,witMtx);
        Mat4Mult(wvpMtx,wMtx,vpMtx);
        
        glUniform3fv(gCamPos, 1, cam->camPosW);
        glUniform3fv(gDiffuseColor,1,curQuad->color);
        glUniformMatrix4fv(gWMtx, 1, GL_FALSE,wMtx);
        glUniformMatrix4fv(gWITMtx, 1, GL_FALSE, witMtx);
        glUniformMatrix4fv(gWVPMtx,1,GL_FALSE,wvpMtx);
        
        glEnableVertexAttribArray(inPosL);
        glEnableVertexAttribArray(inNormL);
        glEnableVertexAttribArray(inTexC);
        glBindBuffer(GL_ARRAY_BUFFER,GetCubeVBO());
        
        glVertexAttribPointer(inPosL,3,GL_FLOAT,GL_FALSE,32,0);
        glVertexAttribPointer(inNormL,3,GL_FLOAT,GL_FALSE,32,12);
        glVertexAttribPointer(inTexC,2,GL_FLOAT, GL_FALSE,32,24);

        glDrawArrays(GL_TRIANGLES, 0, VBO_SIZE_IN_INDICES);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(inPosL);
        glDisableVertexAttribArray(inNormL);
        glDisableVertexAttribArray(inTexC);
    }
    
    glUseProgram(0);
}

void BuildMatricies (Quad *singleQuad, float wMtx[16], float witMtx[16]){
    Quad *q = singleQuad;
    if (singleQuad == NULL || wMtx == NULL || witMtx == NULL) return;
    
    Vec3Normalize(q->rightW);
    Vec3Normalize(q->upW);
    Vec3Normalize(q->lookW);
    
    
}