/*
Copyright 2016 Sean Paget
Email: codeMonkey760@hotmail.com
    
This file is part of gCube.

gCube is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gCube is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with gCube.  If not, see <http://www.gnu.org/licenses/>.
Please read COPYING.txt for details
*/

#ifndef _C_RUBE_CAMERA_H_
#define _C_RUBE_CAMERA_H_

typedef struct {
    float projMtx[16];
    float viewMtx[16];

    float pitchSens;
    float yawSens;
    float zoomSens;
    float pitch;
    float yaw;
    float orbitalRadius;
    bool tracking;
    float camPosW[3];
    
    int lastMousePos[2];
} Camera;

void InitCamera (Camera *cam);
void UpdateCamera (Camera *cam, float dt);
void OnMouseUp (Camera *cam, int pos[2], int button);
void OnMouseDown (Camera *cam, int pos[2], int button);
void OnMouseMove (Camera *cam, int pos[2]);
void RebuildPerspectiveMatrix (Camera *cam, float fovxInDegrees, float aspectRatio, float nearZ, float farZ);
void RebuildOrthographicMatrix (Camera *cam, float left, float right, float bottom, float top, float near, float far);
void Zoom (Camera *cam, float amount);
void CopyProjMtx (Camera *cam, float target[16]);
void CopyViewMtx (Camera *cam, float target[16]);
void CopyAdjustViewMtx (Camera *cam, float target[16]);
void _RefreshViewMtx (Camera *cam);

#endif

