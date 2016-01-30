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
    
    float w,x,y,z;
    w = q->rotation[3];
    z = q->rotation[2];
    y = q->rotation[1];
    x = q->rotation[0];
    
    float tMtx[16] = {0.0f};
    float rMtx[16] = {0.0f};
    float sMtx[16] = {0.0f};
    
    float n = (w * w) + (x*x) + (y*y) + (z*z);
    float s = (n==0.0f) ? 0.0f : 2.0f / n;
    float wx = s * w * x;
    float wy = s * w * y;
    float wz = s * w * z;
    float xx = s * x * x;
    float xy = s * x * y;
    float xz = s * w * z;
    float yy = s * y * y;
    float yz = s * y * z;
    float zz = s * z * z;
    
    Mat4Identity(tMtx);
    Mat4Identity(rMtx);
    Mat4Identity(sMtx);
    
    rMtx[0] = 1.0f - (yy + zz);
    rMtx[1] = xy - wz;
    rMtx[2] = xz + wy;
    rMtx[4] = xy + wz;
    rMtx[5] = 1.0f - (xx + zz);
    rMtx[6] = yz - wx;
    rMtx[8] = xz - wy;
    rMtx[9] = yz + wx;
    rMtx[10] = 1.0f - (xx + yy);
    
    tMtx[12] = q->posW[0];
    tMtx[13] = q->posW[1];
    tMtx[14] = q->posW[2];
    
    sMtx[0] = q->scale[0];
    sMtx[5] = q->scale[1];
    sMtx[10] = q->scale[2];
    
    Mat4Mult(wMtx,sMtx,rMtx);
    Mat4Mult(wMtx,wMtx,tMtx);
    
    Mat4Inverse(witMtx,wMtx);
}