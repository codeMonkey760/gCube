#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Util.h"
#include "Camera.h"

/*
 * typedef struct {
	float projMtx[16];
	float viewMtx[16];

	float pitchSens;
	float yawSens;
	float pitch;
	float yaw;
	float orbitalRadius;
	bool tracking;
	int lastPos[2];
	float camPosW[3];
} Camera;
 */

void InitCamera (Camera *cam) {
	if (cam == NULL) return;

	memset(cam,1,sizeof(Camera));
	Mat4Identity(cam->viewMtx);
	Mat4Identity(cam->projMtx);
	cam->pitchSens = 1.0f;
	cam->yawSens = 1.0f;
	cam->orbitalRadius = 1.0f;
	cam->camPosW[2] = 1.0f;
}

void Update (Camera *cam) {
	;
}

void OnMouseUp (Camera *cam, int pos[2], int button) {
	if (cam == NULL) return;

	cam->tracking = false;
	cam->lastPos[0] = -1;
	cam->lastPos[1] = -1;
}

void OnMouseDown (Camera *cam, int pos[2], int button) {
	if (cam == NULL) return;

	cam->tracking = true;
	cam->lastPos[0] = pos[0];
	cam->lastPos[1] = pos[1];
}

void OnMouseMove (Camera *cam, int pos[2]) {
	int deltaMousePos[2] = {0};
	float camPosW[4] = {0.0f,0.0f,0.0f,1.0f};
	float rightW[4]  = {1.0f,0.0f,0.0f,0.0f};
	float upW[4]     = {0.0f,1.0f,0.0f,0.0f};
	float lookW[4]   = {0.0f,0.0f,1.0f,0.0f};
	float rotX[16]   = {0.0f};
	float rotY[16]   = {0.0f};
	float rotXY[16]  = {0.0f};

	if (cam == NULL || cam->tracking == false) return;

	if (cam->lastPos[0] != -1 && cam->lastPos[1] != -1) {
		deltaMousePos[0] = pos[0] - cam->lastPos[0];
		deltaMousePos[1] = pos[1] - cam->lastPos[1];
	}

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

	cam->lastPos[0] = pos[0];
	cam->lastPos[1] = pos[1];

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
	Vec3Copy(&(cam->viewMtx[0]),rightW);
	Vec3Copy(&(cam->viewMtx[4]),upW);
	Vec3Copy(&(cam->viewMtx[8]),lookW);
	cam->viewMtx[12] = -1.0f * Vec3Dot(rightW,camPosW);
	cam->viewMtx[13] = -1.0f * Vec3Dot(upW,camPosW);
	cam->viewMtx[14] = -1.0f * Vec3Dot(lookW,camPosW);

	Vec3Copy(cam->camPosW,camPosW);
}

void RebuildPerspectiveMatrix (Camera *cam, float fovxInDegrees, float aspectRatio, float nearZ, float farZ) {
	;
}

void RebuildOrthographicsMatrix (Camera *cam, float left, float right, float bottom, float top, float near, float far) {
	;
}

void Zoom (Camera *cam, float amount) {
	;
}

void CopyProjMtx (Camera *cam, float target[16]) {
	;
}

void CopyViewMtx (Camera *cam, float target[16]) {
	;
}

void CopyAdjustedViewMtx (Camera *cam) {
	;
}
