#ifndef _C_RUBE_UTIL_H_
#define _C_RUBE_UTIL_H_

extern char* APPNAME;

extern char *CubeletVS;
extern char *CubeletFS;
extern char *GUIVS;
extern char *GUIFS;

void Mat4Identity(float target[16]);
void Mat4Mult (float ret[16], float m[16], float n[16]);
void Mat4Print (float a[16]);
void Mat4fPrint (FILE *fd, float a[16]);
void Mat4TranslationFA(float a[16], float v[3]);
void Mat4TranslationF (float a[16], float x, float y, float z);
void Mat4ScalingF (float a[16], float x, float y, float z);
void Mat4ScalingFA(float a[16], float v[3]);
void Mat4RotationX(float a[16], float theta);
void Mat4RotationY(float a[16], float theta);
void Mat4RotationZ(float a[16], float theta);
void Mat4RotationAxisF (float a[16], float x, float y, float z, float theta);
void Mat4RotationAxisFA(float a[16], float v[3], float theta);
void Mat4RotationQuaternionF (float m[16], float x, float y, float z, float w);
void Mat4RotationQuaternionFA(float m[16], float q[4]);
void Mat4Transpose(float dst[16], float src[16]);
bool Mat4Inverse (float dst[16], float src[16]);
void Mat4Vec4Mult(float m[16], float v[4], float res[4]);
void Mat4Copy (float m[16], float n[16]);
void Vec4fPrint (FILE *fd, float v[4]);
void Vec4Print(float v[4]);
float degreesToRadians (float d);
float radiansToDegrees (float r);
void Vec3Add (float dst[3], float a[3], float b[3]);
void Vec3Subtract (float dst[3], float a[3], float b[3]);
void Vec3Copy(float dst[3], float src[3]);
float Vec3Dot(float a[3], float b[3]);
void Vec3Cross(float out[3], float u[3], float v[3]);
void Vec3Scalar (float out[3], float u[3], float s);
void Vec3Normalize (float v[3]);
void PixelsToPercent (float x, float y, float width, float height, float *px, float *py);
void QuaternionVec3Rotation (float v[3], float q[4], float out[3]);
void QuaternionFromAxisAngle (float x, float y, float z, float a, float out[4]);
void QuaternionNormalize(float out[4]);
void QuaternionMult(float m[4], float n[4], float out[4]);

FILE* OpenFile(char *name, char *mode);

void Mat4LookAtLH (float out[16], float camPosW[3], float camTargetW[3], float camUpW[3]);

#endif
