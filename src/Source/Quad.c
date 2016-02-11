#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Camera.h"
#include "Util.h"
#include "Quad.h"
#include "Shader.h"
#include "QuadModel.h"

void InitQuadArray (Quad *array, int numQuads) {
    int i;
    Quad *curQuad = NULL;
    if (array == NULL || numQuads < 1) return;
    
    memset(array,0,sizeof(Quad) * numQuads);
    
    for (i = 0; i < numQuads; ++i) {
        curQuad = &array[i];
        
        // init to 100% scale
        curQuad->scale[0] = 1.0f;
        curQuad->scale[1] = 1.0f;
        curQuad->scale[2] = 1.0f;
        
        // init to opaque black
        curQuad->color[3] = 1.0f;
        
        // identity quaternion
        curQuad->rotation[3] = 1.0f;
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
    
    if (
        array == NULL || 
        numQuads < 1 || 
        shader == INVALID_SHADER_ID || 
        cam == NULL
    ) return;
    
    Mat4Mult(vpMtx,cam->viewMtx,cam->projMtx);
    //Mat4Mult(vpMtx, cam->projMtx, cam->viewMtx);
    
    glUseProgram(shader);
    
    for (i = 0; i < numQuads; ++i) {
        // possible performance suck
        Mat4Identity(wMtx);
        Mat4Identity(witMtx);
        Mat4Identity(wvpMtx);
        
        curQuad = &array[i];
        BuildMatricies(curQuad,wMtx,witMtx);
        Mat4Mult(wvpMtx,wMtx,vpMtx);
        //Mat4Mult(wvpMtx,vpMtx,wMtx);
        
        glUniform3fv(gCamPos, 1, cam->camPosW);
        glUniform3fv(gDiffuseColor,1,curQuad->color);
        //glUniformMatrix4fv(gWMtx, 1, GL_FALSE,wMtx);
        //glUniformMatrix4fv(gWITMtx, 1, GL_FALSE, witMtx);
        //glUniformMatrix4fv(gWVPMtx,1,GL_FALSE,wvpMtx);
        glUniformMatrix4fv(gWMtx, 1, GL_TRUE,wMtx);
        glUniformMatrix4fv(gWITMtx, 1, GL_TRUE, witMtx);
        glUniformMatrix4fv(gWVPMtx,1,GL_TRUE,wvpMtx);
        
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
    
    float tMtx[16] = {0.0f};
    float rMtx[16] = {0.0f};
    float sMtx[16] = {0.0f};
    
    Mat4Scaling(sMtx,q->scale);
    Mat4RotationQuaternion(rMtx,q->rotation);
    Mat4Translation(tMtx,q->posW);
    
    Mat4Mult(wMtx,sMtx,rMtx);
    Mat4Mult(wMtx,wMtx,tMtx);
    
    Mat4Inverse(witMtx,wMtx);
}