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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "Util.h"

char *APPNAME = "gCube";

float degreesToRadians (float d) {
    return d / 180.0f * M_PI;
}

float radiansToDegrees (float r) {
    return r / M_PI * 180.0f;
}

void Mat4Identity (float a[16]) {
    int i;
    if (a == NULL) return;

    for (i = 0; i < 16; ++i) {
            a[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

void Mat4TranslationF (float a[16], float x, float y, float z) {
	float v[4] = {x,y,z,0.0f};

	Mat4TranslationFA(a, v);
}

void Mat4TranslationFA(float a[16], float v[3]) {
    if (a == NULL || v == NULL) return;

    Mat4Identity(a);
    a[12] = v[0];
    a[13] = v[1];
    a[14] = v[2];
}

void Mat4ScalingF (float a[16], float x, float y, float z) {
	if (a == NULL) return;

	Mat4Identity(a);
	a[0] = x;
	a[5] = y;
	a[10] = z;
}

void Mat4ScalingFA(float a[16], float v[3]) {
    if (a == NULL || v == NULL) return;

    Mat4Identity(a);
    a[0] = v[0];
    a[5] = v[1];
    a[10] = v[2];
}

void Mat4RotationX(float a[16], float theta) {
    if (a == NULL) return;

    theta = degreesToRadians(theta);

    float st = sin(theta);
    float ct = cos(theta);

    Mat4Identity(a);
    a[5] = ct;
    a[6] = st;
    a[9] = -st;
    a[10] = ct;
}

void Mat4RotationY(float a[16], float theta) {
    if (a == NULL) return;

    theta = degreesToRadians(theta);

    float ct = cos(theta);
    float st = sin(theta);

    Mat4Identity(a);
    a[0] = ct;
    a[2] = -st;
    a[8] = st;
    a[10] = ct;
}

void Mat4RotationZ(float a[16], float theta) {
    if (a == NULL) return;

    theta = degreesToRadians(theta);

    float ct = cos(theta);
    float st = sin(theta);

    Mat4Identity(a);
    a[0] = ct;
    a[1] = st;
    a[4] = -st;
    a[5] = ct;
}

void Mat4RotationAxisF (float a[16], float x, float y, float z, float theta) {
	float v[4] = {x,y,z,0.0f};

	Mat4RotationAxisFA(a,v,theta);
}

void Mat4RotationAxisFA(float a[16], float v[3], float theta) {
    if (a == NULL || v == NULL) return;

    theta = degreesToRadians(theta);

    float x = v[0];
    float y = v[1];
    float z = v[2];
    float len = sqrt(x*x + y*y + z*z);
    x /= len;
    y /= len;
    z /= len;

    float c = cos(theta);
    float s = sin(theta);
    float nc = 1 - c;
    float xy = x * y;
    float yz = y * z;
    float zx = z * x;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;

    Mat4Identity(a);
    a[0]  = x * x * nc + c;
    a[1]  = xy * nc - zs;
    a[2]  = zx * nc + ys;

    a[4]  = xy * nc + zs;
    a[5]  = y * y * nc + c;
    a[6]  = yz * nc - xs;

    a[8]  = zx * nc - ys;
    a[9]  = yz * nc + xs;
    a[10] = z * z * nc + c;
}

void Mat4RotationQuaternionF (float m[16], float x, float y, float z, float w) {
	float q[4] = {x,y,z,w};

	Mat4RotationQuaternionFA(m,q);
}

void Mat4RotationQuaternionFA(float m[16], float q[4]) {
    if (m == NULL || q == NULL) return;
    
    float x = q[0];
    float y = q[1];
    float z = q[2];
    float w = q[3];
    
    float n = (x * x) + (y * y) + (z * z) + (w * w);
    float s = (n == 0.0f) ? 0.0f : 2.0f / n;
    float wx = s * w * x;
    float wy = s * w * y;
    float wz = s * w * z;
    float xx = s * x * x;
    float xy = s * x * y;
    float xz = s * x * z;
    float yy = s * y * y;
    float yz = s * y * z;
    float zz = s * z * z;
    
    Mat4Identity(m);
    
    /*
    m[0]  = 1.0f - (yy + zz);
    m[1]  = xy - wz;
    m[2]  = xz + wy;
    m[4]  = xy + wz;
    m[5]  = 1.0f - (xx + zz);
    m[6]  = yz - wx;
    m[8]  = xz - wy;
    m[9]  = yz + wx;
    m[10] = 1.0f - (xx + yy);
    */
    
    m[0]  = 1.0f - (yy + zz);
    m[4]  = xy - wz;
    m[8]  = xz + wy;
    m[1]  = xy + wz;
    m[5]  = 1.0f - (xx + zz);
    m[9]  = yz - wx;
    m[2]  = xz - wy;
    m[6]  = yz + wx;
    m[10] = 1.0f - (xx + yy);
}

void Mat4Transpose (float dst[16], float src[16]) {
    int i,j;
    float temp[16] = {0.0f};
    if (dst == NULL || src == NULL) return;
    
    // copy to a temp matrix just in case this is an 'in place'
    // transpose ... ie: dst == src
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            temp[i*4+j] = src[j*4+i];
        }
    }
    
    memcpy(dst,temp,sizeof(float) * 16);
}

// lifted this from:
//http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
// seems to be a copy of bool gluInvertMatrix(const double m[16], double invOut[16])
bool Mat4Inverse(float invOut[16], float m[16]) {
    float inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

// non gsl version
void Mat4Mult (float ret[16], float m[16], float n[16]) {
    int i;
    float temp[16] = {0.0f};
    
    if (ret == NULL || m == NULL || n == NULL) return;
    
    temp[ 0] = (m[ 0] * n[ 0]) + (m[ 1] * n[ 4]) + (m[ 2] * n[ 8]) + (m[ 3] * n[12]);
    temp[ 1] = (m[ 0] * n[ 1]) + (m[ 1] * n[ 5]) + (m[ 2] * n[ 9]) + (m[ 3] * n[13]);
    temp[ 2] = (m[ 0] * n[ 2]) + (m[ 1] * n[ 6]) + (m[ 2] * n[10]) + (m[ 3] * n[14]);
    temp[ 3] = (m[ 0] * n[ 3]) + (m[ 1] * n[ 7]) + (m[ 2] * n[11]) + (m[ 3] * n[15]);
    
    temp[ 4] = (m[ 4] * n[ 0]) + (m[ 5] * n[ 4]) + (m[ 6] * n[ 8]) + (m[ 7] * n[12]);
    temp[ 5] = (m[ 4] * n[ 1]) + (m[ 5] * n[ 5]) + (m[ 6] * n[ 9]) + (m[ 7] * n[13]);
    temp[ 6] = (m[ 4] * n[ 2]) + (m[ 5] * n[ 6]) + (m[ 6] * n[10]) + (m[ 7] * n[14]);
    temp[ 7] = (m[ 4] * n[ 3]) + (m[ 5] * n[ 7]) + (m[ 6] * n[11]) + (m[ 7] * n[15]);
    
    temp[ 8] = (m[ 8] * n[ 0]) + (m[ 9] * n[ 4]) + (m[10] * n[ 8]) + (m[11] * n[12]);
    temp[ 9] = (m[ 8] * n[ 1]) + (m[ 9] * n[ 5]) + (m[10] * n[ 9]) + (m[11] * n[13]);
    temp[10] = (m[ 8] * n[ 2]) + (m[ 9] * n[ 6]) + (m[10] * n[10]) + (m[11] * n[14]);
    temp[11] = (m[ 8] * n[ 3]) + (m[ 9] * n[ 7]) + (m[10] * n[11]) + (m[11] * n[15]);
    
    temp[12] = (m[12] * n[ 0]) + (m[13] * n[ 4]) + (m[14] * n[ 8]) + (m[15] * n[12]);
    temp[13] = (m[12] * n[ 1]) + (m[13] * n[ 5]) + (m[14] * n[ 9]) + (m[15] * n[13]);
    temp[14] = (m[12] * n[ 2]) + (m[13] * n[ 6]) + (m[14] * n[10]) + (m[15] * n[14]);
    temp[15] = (m[12] * n[ 3]) + (m[13] * n[ 7]) + (m[14] * n[11]) + (m[15] * n[15]);
    
    for (i = 0; i < 16; ++i) {
        ret[i] = temp[i];
    }
}

void Mat4Print (float a[16]) {
    Mat4fPrint (stdout, a);
}

void Mat4fPrint (FILE *fd, float a[16]) {
    int i;
    if (fd == NULL || a == NULL) return;

    for (i = 0; i < 16; ++i) {
            fprintf(fd,"%.2f %s",a[i],((i+1) % 4 ==0 ? "\n" : ""));
    }
}

void Mat4Vec4Mult (float m[16], float v[4], float ret[4]) {
    float temp[4] = {0.0f};
    
    if (m == NULL || v == NULL || ret == NULL) return;
    
    temp[0] = (m[ 0] * v[0]) + (m[ 4] * v[1]) + (m[ 8] * v[2]) + (m[12] * v[3]);
    temp[1] = (m[ 1] * v[0]) + (m[ 5] * v[1]) + (m[ 9] * v[2]) + (m[13] * v[3]);
    temp[2] = (m[ 2] * v[0]) + (m[ 6] * v[1]) + (m[10] * v[2]) + (m[14] * v[3]);
    temp[3] = (m[ 3] * v[0]) + (m[ 7] * v[1]) + (m[11] * v[2]) + (m[15] * v[3]);
    
    ret[0] = temp[0];
    ret[1] = temp[1];
    ret[2] = temp[2];
    ret[3] = temp[3];
}

void Mat4Copy (float dst[16], float src[16]) {
    int i;
    if (dst == NULL || src == NULL) return;
    
    for (i = 0; i < 16; ++i) {
        dst[i] = src[i];
    }
}

void Vec4fPrint(FILE *fd, float v[4]) {
    int i;
    if (fd == NULL || v == NULL) return;

    for (i = 0; i < 4; ++i) {
            fprintf(fd, "%.2f ", v[i]);
    }
    fprintf(fd,"\n");
}

void Vec4Print(float v[4]) {
    Vec4fPrint(stdout, v);
}

void Vec3Add (float out[3], float a[3], float b[3]) {
    int i;
    if (out == NULL || a == NULL || b == NULL) return;
    
    for (i = 0; i < 3; ++i) {
        out[i] = a[i] + b[i];
    }
}

void Vec3Subtract (float out[3], float a[3], float b[3]) {
    int i;
    if (out == NULL || a == NULL || b == NULL) return;
    
    for (i = 0; i < 3; ++i) {
        out[i] = a[i] - b[i];
    }
}

void Vec3Copy (float dst[3], float src[3]) {
    int i;
    if (dst == NULL || src == NULL) return;
    
    for (i = 0; i < 3; ++i) {
        dst[i] = src[i];
    }
}

float Vec3Dot (float a[3], float b[3]) {
    if (a == NULL || b == NULL) return 0.0f;
    
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

void Vec3Normalize (float v[3]) {
    float len;
    
    if (v == NULL) return;
    
    len = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

void Vec3Cross (float out[3], float u[3], float v[3]) {
    if (u == NULL || v == NULL || out == NULL) return;
    float temp[3] = {0.0f};
    int i;
    
    temp[0] = (u[1] * v[2]) - (u[2] * v[1]);
    temp[1] = (u[2] * v[0]) - (u[0] * v[2]);
    temp[2] = (u[0] * v[1]) - (u[1] * v[0]);
    
    for (i = 0; i < 3; ++i) {
        out[i] = temp[i];
    }
}

void Vec3Scalar (float out[3], float u[3], float s) {
    if (u == NULL || out == NULL) return;
    int i;
    
    for (i = 0; i < 3; ++i) {
        out[i] = u[i] * s;
    }
}

void QuaternionVec3Rotation (float v[3], float q[4], float out[3]) {
    if (v == NULL || q == NULL || out == NULL) return;
    
    float temp[3] = {0.0f};
    float temp2[3] = {0.0f};
    float temp3[3] = {0.0f};

    int i;
    Vec3Scalar(temp,q,(2.0f * Vec3Dot(q,v)));
    Vec3Scalar(temp2,v,((q[3] * q[3]) - Vec3Dot(q,q)));
    Vec3Cross(temp3,q,v);
    Vec3Scalar(temp3,temp3,2.0f * q[3]);
    
    for (i = 0; i < 3; ++i) {
        out[i] = temp[i] + temp2[i] + temp3[i];
    }
}

// found this at:
// http://stackoverflow.com/questions/4436764/rotating-a-quaternion-on-1-axis
void QuaternionFromAxisAngle(float x, float y, float z, float a, float out[4]) {
    if (out == NULL) return;
    
    a = degreesToRadians(a);
    
    float fac = sin (a / 2.0f);
    
    out[0] = x * fac;
    out[1] = y * fac;
    out[2] = z * fac;
    
    out[3] = cos(a / 2.0f);
    
    QuaternionNormalize(out);
}

void QuaternionNormalize(float out[4]) {
    if (out == NULL) return;
    
    float len = (out[0] * out[0]) + (out[1] * out[1]) + (out[2] * out[2]) + (out[3] * out[3]);
    len = sqrt(len);
    
    out[0] /= len;
    out[1] /= len;
    out[2] /= len;
    out[3] /= len;
}

/*
 * http://www.cprogramming.com/tutorial/3d/quaternions.html
 Let Q1 and Q2 be two quaternions, which are defined, respectively, as (w1, x1, y1, z1) and (w2, x2, y2, z2).
(Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
(Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
(Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2)
(Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
*/

void QuaternionMult(float m[4], float n[4], float out[4]) {
    int i;
    if (m == NULL || n == NULL || out == NULL) return;
    
    float temp[4] = {0.0f};
    temp[0] = (m[3] * n[0]) + (m[0] * n[3]) + (m[1] * n[2]) - (m[2] * n[1]);
    temp[1] = (m[3] * n[1]) - (m[0] * n[2]) + (m[1] * n[3]) + (m[2] * n[0]);
    temp[2] = (m[3] * n[2]) + (m[0] * n[1]) - (m[1] * n[0]) + (m[2] * n[3]);
    temp[3] = (m[3] * n[3]) - (m[0] * n[0]) - (m[1] * n[1]) - (m[2] * n[2]);
    
    for (i = 0; i < 4; ++i) {
        out[i] = temp[i];
    }
}

void Mat4LookAtLH (float out[16], float camPosW[3], float camTargetW[3], float camUpW[3]) {
    if (
        out == NULL ||
        camPosW == NULL ||
        camTargetW == NULL ||
        camUpW == NULL
    ) {
        return;
    }
    
    float look[3] = {0.0f};
    float up[3] = {0.0f};
    float right[3] = {0.0f};
    float negCamPosW[3] = {0.0f};
    
    Mat4Identity(out);
    
    Vec3Subtract(look,camTargetW,camPosW);
    Vec3Normalize(look);
    
    Vec3Cross(right, camUpW, look);
    Vec3Normalize(right);
    
    Vec3Cross(up,look,right);
    Vec3Normalize(up);
    
    Vec3Scalar(negCamPosW, camPosW, -1.0f);
    
    out[ 0] = right[0];
    out[ 1] = up[0];
    out[ 2] = look[0];
    out[ 3] = 0.0f;
    
    out[ 4] = right[1];
    out[ 5] = up[1];
    out[ 6] = look[1];
    out[ 7] = 0.0f;
    
    out[ 8] = right[2];
    out[ 9] = up[2];
    out[10] = look[2];
    out[11] = 0.0f;
    
    out[12] = Vec3Dot(negCamPosW,right);
    out[13] = Vec3Dot(negCamPosW,up);
    out[14] = Vec3Dot(negCamPosW,look);
    out[15] = 1.0f;
}

/*
 Platform independent file opening routine
*/
FILE* OpenFile (char *name, char *mode) {
    char path[4096] = {0};
    char *basePath = NULL;
    FILE *file = NULL;
    
    memset(path,0,sizeof(char) * 4096);
    basePath = getenv("HOME");
    if (basePath != NULL) {
        strcpy(path,basePath);
        strcat(path,"/.gCube/");
        strcat(path,name);
        file = fopen(path,mode);
        if (file != NULL) {
        	printf("Found file: %s\n", path);
        	return file;
        } else {
        	printf("Did not find file: %s\n",path);
        }
    }
    
    memset(path,0,sizeof(char) * 4096);
    basePath = getenv("HOMEPATH");
    if (basePath != NULL) {
        strcpy(path,basePath);
        strcat(path,"\\.gCube\\");
        strcat(path,name);
        file = fopen(path,mode);
        if (file != NULL) {
        	printf("Found file: %s\n", path);
        	return file;
        } else {
        	printf("Did not find file: %s\n",path);
        }
    }
    
    memset(path,0,sizeof(char) * 4096);
    strcpy(path,name);
    file = fopen(path,mode);
    
    if (file != NULL) {
        printf("Found file: %s\n", path);
    } else {
        printf("Did not find file: %s\n",path);
    }
    
    return file;
}

/*
	Checks for the existence of .gCube dir in home dir
	Attempts to create it if it doesn't exist
*/
void CheckHomeDir (void) {
	char path[4096] = {0};
	char *basepath = NULL;
	struct stat sb;
	
#ifdef __linux__
	basepath = getenv("HOME");
	if (basepath != NULL) {
		fprintf(stdout,"%s\n",basepath);
		strcpy(path,basepath);
		strcat(path, "/.gCube");
	} else return;
#elif defined(_WIN32)
	basepath = getenv("HOMEPATH");
	if (basepath != NULL) {
		fprintf(stdout,"%s\n",basepath);
		strcpy(path,basepath);
		strcat(path, "\\.gCube");
	} else return;
#endif

	if (!(stat(path,&sb) == 0 && S_ISDIR(sb.st_mode))) {
		if (mkdir(path,0777) == -1) {
			fprintf(stderr, "Could not create a gcube dir on home path!\n%s\n",path);
		} else {
			printf("Created directory: %s\n",path);
		}
	}
}

char *CubeletVS = 
"#version 440\n\n"

"in vec3 posL;\n"
"in vec3 normL;\n"
"in vec2 intexC;\n\n"

"uniform mat4 gWMtx;\n"
"uniform mat4 gWITMtx;\n"
"uniform mat4 gWVPMtx;\n"
"uniform mat3 gTexMtx;\n\n"

"out vec3 posW;\n"
"out vec3 normW;\n"
"out vec2 outtexC;\n\n"

"void main (void) {\n"
"    posW = (vec4(posL,1.0f) * gWMtx).xyz;\n"
"    normW = (vec4(normL,0.0f) * gWITMtx).xyz;\n"
"    outtexC = (vec3(intexC,1.0f) * gTexMtx).xy;\n\n"
    
"    gl_Position = (vec4(posL,1.0f) * gWVPMtx);\n"
"}";

char *CubeletFS = 
"#version 440\n\n"

"in vec3 posW;\n"
"in vec3 normW;\n"
"in vec2 outtexC;\n\n"

"uniform vec3 gDiffuseColor;\n"
"uniform vec3 gCamPos;\n\n"

"uniform sampler2D gTexture;\n\n"

"void main (void) {\n"
"    vec3 toCam = normalize(gCamPos - posW);\n"
"    vec3 normW2 = normalize(normW);\n\n"

"    float diffuse = max(dot(toCam,normW2),0.0f);\n"
"    diffuse = (diffuse * 0.7f) + 0.3f;\n\n"
    
"    vec3 col = texture(gTexture, outtexC).rgb * gDiffuseColor;\n\n"

"    gl_FragColor = vec4((col * diffuse),1.0f);\n"
"}";

char *GUIVS = 
"#version 440\n\n"

"in vec3 posL;\n"
"in vec2 intexC;\n\n"

"uniform mat4 gTMtx;\n"
"uniform mat3 gTexMtx;\n\n"

"out vec2 outTexC;\n\n"

"void main (void) {\n"
"    gl_Position = vec4(posL,1.0f) * gTMtx;\n\n"

"    outTexC = (vec3(intexC,1.0f) * gTexMtx).xy;\n"
"}";

char *GUIFS = 
"#version 440\n\n"

"in vec2 outTexC;\n\n"

"uniform sampler2D gTexture;\n"
"uniform vec3 gAmbient;\n"
"uniform float gMixValue;\n\n"

"void main (void) {\n"
"    vec4 tex = texture2D(gTexture, outTexC).rgba;\n"
"    float a = tex.a;\n\n"

"    vec3 col = tex.rgb;\n"
"    col = (gMixValue * col) + (gAmbient * (1.0f - gMixValue));\n\n"

"    gl_FragColor = vec4(col,a * gMixValue);\n"
"}\n";

