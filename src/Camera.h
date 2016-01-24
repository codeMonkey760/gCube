#ifndef _C_RUBE_CAMERA_H_
#define _C_RUBE_CAMERA_H_

typedef struct {
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

void InitCamera (Camera *cam);
void Update (Camera *cam);
void OnMouseUp (Camera *cam, int pos[2], int button);
void OnMouseDown (Camera *cam, int pos[2], int button);
void OnMouseMove (Camera *cam, int pos[2]);
void RebuildPerspectiveMatrix (Camera *cam, float fovxInDegrees, float aspectRatio, float nearZ, float farZ);
void RebuildOrthographicMatrix (Camera *cam, float left, float right, float bottom, float top, float near, float far);
void Zoom (Camera *cam, float amount);
void CopyProjMtx (Camera *cam, float target[16]);
void CopyViewMtx (Camera *cam, float target[16]);
void CopyAdjustViewMtx (Camera *cam);

#endif
