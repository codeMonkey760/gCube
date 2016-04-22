#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "Util.h"
#include "Camera.h"
#include "Config.h"

void InitCamera (Camera *cam) {
    if (cam == NULL) return;

    memset(cam,0,sizeof(Camera));
    Mat4Identity(cam->viewMtx);
    Mat4Identity(cam->projMtx);
    cam->pitchSens = CONFIG_defaultPitchSens;
    cam->yawSens = CONFIG_defaultYawSens;
    cam->zoomSens = CONFIG_defaultZoomSens;
    cam->orbitalRadius = CONFIG_defaultOrbitalRadius;
    cam->lastMousePos[0] = -1;
    cam->lastMousePos[1] = -1;
    
    _RefreshViewMtx(cam);
}

void UpdateCamera (Camera *cam, float dt) {
    ;
}

void OnMouseUp (Camera *cam, int pos[2], int button) {
    if (cam == NULL) return;
    
    cam->lastMousePos[0] = -1;
    cam->lastMousePos[1] = -1;

    cam->tracking = false;
}

void OnMouseDown (Camera *cam, int pos[2], int button) {
    if (cam == NULL || button != 1) return;

    cam->tracking = true;
    cam->lastMousePos[0] = pos[0];
    cam->lastMousePos[1] = pos[1];
}

void OnMouseMove (Camera *cam, int pos[2]) {
    int deltaMousePos[2] = {0};
    if (
        cam == NULL || 
        pos == NULL ||
        cam->tracking == false
    ) return;
    
    deltaMousePos[0] = pos[0] - cam->lastMousePos[0];
    deltaMousePos[1] = pos[1] - cam->lastMousePos[1];
    
    cam->lastMousePos[0] = pos[0];
    cam->lastMousePos[1] = pos[1];
    
    cam->yaw += ((float) deltaMousePos[0]) * cam->yawSens;
    while (cam->yaw >= 360.0f) {
            cam->yaw -= 360.0f;
    }
    while (cam->yaw < 0.0f) {
            cam->yaw += 360.0f;
    }

    cam->pitch += ((float) deltaMousePos[1]) * cam->pitchSens;
    if (cam->pitch > 88.0f) {
            cam->pitch = 88.0f;
    }
    if (cam->pitch < - 88.0f) {
            cam->pitch = -88.0f;
    }

    _RefreshViewMtx(cam);
}

void RebuildPerspectiveMatrix (Camera *cam, float fovxInDegrees, float aspectRatio, float nearZ, float farZ) {
    fovxInDegrees = fovxInDegrees * (1.0f / aspectRatio);
    float fovyInRadians = degreesToRadians(fovxInDegrees);
    
    float w = 1.0f / (aspectRatio * tan(fovyInRadians / 2.0f));
    float h = 1.0f / (tan(fovyInRadians / 2.0f));
    
    if (cam == NULL) return;
    Mat4Identity(cam->projMtx);
    cam->projMtx[0] = w;
    cam->projMtx[5] = h;
    cam->projMtx[10] = (farZ) / (farZ - nearZ);
    cam->projMtx[11] = 1.0f;
    cam->projMtx[14] = (-1.0f * nearZ * farZ) / (farZ - nearZ);
}

void RebuildOrthographicMatrix (Camera *cam, float left, float right, float bottom, float top, float near, float far) {
    if (cam == NULL) return;
    
    Mat4Identity(cam->projMtx);
    cam->projMtx[0] = 2.0f / (right - left);
    cam->projMtx[5] = 2.0f / (top - bottom);
    cam->projMtx[10] = 1.0f / (far - near);
    cam->projMtx[14] = near / (near - far);
}

void Zoom (Camera *cam, float amount) {
    if (cam == NULL) return;
    
    amount *= cam->zoomSens;
    
    cam->orbitalRadius -= amount;
    if (cam->orbitalRadius < CONFIG_minZoom) cam->orbitalRadius = CONFIG_minZoom;
    if (cam->orbitalRadius > CONFIG_maxZoom) cam->orbitalRadius = CONFIG_maxZoom;
    
    _RefreshViewMtx(cam);
}

void CopyProjMtx (Camera *cam, float target[16]) {
    if (cam == NULL || target == NULL) return;
    
    memcpy(target,cam->projMtx,sizeof(float) * 16);
}

void CopyViewMtx (Camera *cam, float target[16]) {
    if (cam == NULL || target == NULL) return;
    
    memcpy(target,cam->viewMtx,sizeof(float) * 16);
}

void CopyAdjustedViewMtx (Camera *cam, float target[16]) {
    float camDirs[3][4] = {
        {1.0f,0.0f,0.0f,0.0f},
        {0.0f,1.0f,0.0f,0.0f},
        {0.0f,0.0f,1.0f,0.0f}
    };
    int i = 0;
    int d = 0;
    
    float max = 0.0f;
    int axisId = -1;
    bool neg = false;
    
    if (cam == NULL || target == NULL) return;
    
    //option 2 for view matrix decomposition
    for (i = 0; i < 3; ++i) {
        for (d = 0; d < 3; ++d) {
            camDirs[i][d] = cam->viewMtx[d*4+i];
        }
    }
    
    // for all directional vectors
    for (i = 0; i < 3; ++i) {
        // reset locals
        max = 0.0f;
        axisId = -1;
        neg = false;
        
        // for all xyz components in current directional vector
        for (d = 0; d < 3; ++d) {
            // check if current component is the longest
            // remember it if it is
            if (camDirs[i][d] > max) {
                max = camDirs[i][d];
                axisId = d;
                neg = false;
            } else if (fabs(camDirs[i][d]) > max) {
                max = fabs(camDirs[i][d]);
                axisId = d;
                neg = true;
            }
            // set all components to zero
            camDirs[i][d] = 0.0f;
        }
        // reset the largest component to one or neg one
        camDirs[i][axisId] = (neg) ? -1.0f : 1.0f;
    }
    
    // rebuild a view matrix using the clamped directional vectors
    Mat4Identity(target);
    for (i = 0; i < 3; ++i) {
        for (d = 0; d < 3; ++d) {
            target[d*4+i] = camDirs[i][d];
        }
    }
}

void _RefreshViewMtx (Camera *cam) {
    float camPosW[3] = {0.0f,0.0f,0.0f};
    float camTargetW[3] = {0.0f,0.0f,0.0f};
    float camUpW[3] = {0.0f,1.0f,0.0f};
    
    camPosW[2] = -1.0f * cam->orbitalRadius;
    
    float qY[4] = {0.0f};
    float qX[4] = {0.0f};
    float qTot[4] = {0.0f};
    
    QuaternionFromAxisAngle(0.0f,1.0f,0.0f,cam->yaw,qY);
    QuaternionFromAxisAngle(1.0f,0.0f,0.0f,cam->pitch,qX);
    QuaternionMult(qY,qX,qTot);
    
    QuaternionVec3Rotation(camPosW,qTot,camPosW);
    
    Mat4LookAtLH(cam->viewMtx,camPosW,camTargetW,camUpW);
    
    Vec3Copy(cam->camPosW,camPosW);
    
    /*
    float camPosW[4] = {0.0f,0.0f,0.0f,1.0f};
    float negCamPosW[4] = {0.0f,0.0f,0.0f,1.0f};
    float rightW[4]  = {1.0f,0.0f,0.0f,0.0f};
    float upW[4]     = {0.0f,1.0f,0.0f,0.0f};
    float lookW[4]   = {0.0f,0.0f,1.0f,0.0f};
    float rotX[16]   = {0.0f};
    float rotY[16]   = {0.0f};
    float rotXY[16]  = {0.0f};
    int i;

    if (cam == NULL) return;
    
    camPosW[2] = -1.0f * cam->orbitalRadius;
    Mat4Identity(rotX);
    Mat4Identity(rotY);
    Mat4Identity(rotXY);
    Mat4RotationX(rotX,-1.0f * cam->pitch);
    Mat4RotationY(rotY,-1.0f * cam->yaw);
    Mat4Mult(rotXY,rotX,rotY);
    Mat4Vec4Mult(rotXY,camPosW,camPosW);
    Mat4Vec4Mult(rotXY,rightW,rightW);
    Mat4Vec4Mult(rotXY,upW,upW);
    Mat4Vec4Mult(rotXY,lookW,lookW);

    Mat4Identity(cam->viewMtx);
    cam->viewMtx[0]  = rightW[0];
    cam->viewMtx[1]  = upW[0];
    cam->viewMtx[2]  = lookW[0];
    
    cam->viewMtx[4]  = rightW[1];
    cam->viewMtx[5]  = upW[1];
    cam->viewMtx[6]  = lookW[1];
    
    cam->viewMtx[8]  = rightW[2];
    cam->viewMtx[9]  = upW[2];
    cam->viewMtx[10] = lookW[2];
    
    //cam->viewMtx[12] = -1.0f * Vec3Dot(rightW,camPosW);
    //cam->viewMtx[13] = -1.0f * Vec3Dot(upW,camPosW);
    //cam->viewMtx[14] = -1.0f * Vec3Dot(lookW,camPosW);
    
    Vec3Copy(negCamPosW,camPosW);
    for (i = 0; i < 3; ++i) {
        negCamPosW[i] *= -1.0f;
    }
    
    cam->viewMtx[12] = Vec3Dot(negCamPosW,rightW);
    cam->viewMtx[13] = Vec3Dot(negCamPosW,upW);
    cam->viewMtx[14] = Vec3Dot(negCamPosW,lookW);
    
    cam->viewMtx[12] = -1.0f * Vec3Dot(camPosW,rightW);
    cam->viewMtx[13] = -1.0f * Vec3Dot(camPosW,upW);
    cam->viewMtx[14] = -1.0f * Vec3Dot(camPosW,lookW);

    Vec3Copy(cam->camPosW,camPosW);
    
    // Test Code
    //cam->camPosW[0] = 0.0f;
    //cam->camPosW[1] = 0.0f;
    //cam->camPosW[2] = 0.0f;
    //Mat4Identity(cam->viewMtx);
    //cam->viewMtx[12] = 0.0f;
    //cam->viewMtx[14] = 0.0f;
    */
}