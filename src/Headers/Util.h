#ifndef _C_RUBE_UTIL_H_
#define _C_RUBE_UTIL_H_

void Mat4Identity(float target[]);
void Mat4Mult (float a[], float b[], float c[]);
void Mat4Print (float a[]);
void Mat4fPrint (FILE *fd, float a[]);
void Mat4Translation(float a[], float v[]);
void Mat4Scaling(float a[], float v[]);
void Mat4RotationX(float a[], float theta);
void Mat4RotationY(float a[], float theta);
void Mat4RotationZ(float a[], float theta);
void Mat4RotationAxis(float a[], float v[], float theta);
void Mat4RotationQuaternion (float m[], float q[]);
void Mat4Transpose(float dst[16], float src[16]);
bool Mat4Inverse (float dst[16], float src[16]);
void Mat4Vec4Mult(float m[], float v[], float r[]);
void Vec4fPrint (FILE *fd, float v[]);
void Vec4Print(float v[]);
float degreesToRadians (float d);
float radiansToDegrees (float r);
void Vec3Copy(float *dst, float *src);
float Vec3Dot(float a[], float b[]);

void Vec3Normalize (float v[3]);
void PixelsToPercent (float x, float y, float width, float height, float *px, float *py);
void QuaternionFromAxisAngle (float x, float y, float z, float a, float out[4]);
void QuaternionNormalize(float out[4]);
void QuaternionMult(float m[4], float n[4], float out[4]);

#endif
