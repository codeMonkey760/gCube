#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>

#include "Util.h"

float degreesToRadians (float d) {
    return d / 180.0f * M_PI;
}

float radiansToDegrees (float r) {
    return r / M_PI * 180.0f;
}

void Mat4Identity (float a[]) {
    int i;
    if (a == NULL) return;

    for (i = 0; i < 16; ++i) {
            a[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

void Mat4Translation (float a[], float v[]) {
    if (a == NULL || v == NULL) return;

    Mat4Identity(a);
    //a[3] = v[0];
    //a[7] = v[1];
    //a[11] = v[2];
    a[12] = v[0];
    a[13] = v[1];
    a[14] = v[2];
}

void Mat4Scaling (float a[], float v[]) {
    if (a == NULL || v == NULL) return;

    Mat4Identity(a);
    a[0] = v[0];
    a[5] = v[1];
    a[10] = v[2];
}

void Mat4RotationX(float a[], float theta) {
    if (a == NULL) return;

    theta = degreesToRadians(theta);

    float st = sin(theta);
    float ct = cos(theta);

    Mat4Identity(a);
    a[5] = ct;
    a[6] = -st;
    a[9] = st;
    a[10] = ct;
}

void Mat4RotationY(float a[], float theta) {
    if (a == NULL) return;

    theta = degreesToRadians(theta);

    float ct = cos(theta);
    float st = sin(theta);

    Mat4Identity(a);
    a[0] = ct;
    a[2] = st;
    a[8] = -st;
    a[10] = ct;
}

void Mat4RotationZ(float a[], float theta) {
    if (a == NULL) return;

    theta = degreesToRadians(theta);

    float ct = cos(theta);
    float st = sin(theta);

    Mat4Identity(a);
    a[0] = ct;
    a[1] = -st;
    a[4] = st;
    a[5] = ct;
}

void Mat4RotationAxis (float a[], float v[], float theta) {
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
    a[1]  = xy * nc + zs;
    a[2]  = zx * nc - ys;

    a[4]  = xy * nc - zs;
    a[5]  = y * y * nc + c;
    a[6]  = yz * nc + xs;

    a[8]  = zx * nc + ys;
    a[9]  = yz * nc - xs;
    a[10] = z * z * nc + c;
}

void Mat4RotationQuaternion (float m[], float q[]) {
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
    
    m[0]  = 1.0f - (yy + zz);
    m[1]  = xy - wz;
    m[2]  = xz + wy;
    m[4]  = xy + wz;
    m[5]  = 1.0f - (xx + zz);
    m[6]  = yz - wx;
    m[8]  = xz - wy;
    m[9]  = yz + wx;
    m[10] = 1.0f - (xx + yy);
}

// Check this out when I get a chance
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

/*
// this too
void Mat4Inverse (float dst[16], float src[16]) {
    float temp[16] = {0.0f};
    if (dst == NULL || src == NULL) return;
    
    gsl_matrix_float_view D = gsl_matrix_float_view_array(dst,4,4);
    gsl_matrix_float_view S = gsl_matrix_float_view_array(src,4,4);
    gsl_matrix_float_view T = gsl_matrix_float_view_array(temp,4,4);
    gsl_permutation *p = gsl_permutation_alloc(4);
    int s;
    
    gluInvertMatrix
    gsl_linalg_LU_decomp (&S.matrix, p, &s);
    gsl_linalg_LU_invert (&S.matrix, p, &T.matrix);
    
    memcpy(dst,temp, sizeof(float) * 16);
}
 */

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

void Mat4Mult (float c[], float a[], float b[]) {
    float temp[16] = {0.0f};

    if (a == NULL || b == NULL || c == NULL) return;

    gsl_matrix_float_view A = gsl_matrix_float_view_array(a,4,4);
    gsl_matrix_float_view B = gsl_matrix_float_view_array(b,4,4);
    gsl_matrix_float_view C = gsl_matrix_float_view_array(c,4,4);
    gsl_matrix_float_view T = gsl_matrix_float_view_array(temp,4,4);

    gsl_blas_sgemm (CblasNoTrans, CblasNoTrans, 1.0f, &A.matrix, &B.matrix, 0.0f, &T.matrix);
    gsl_matrix_float_memcpy(&C.matrix, &T.matrix);
}

void Mat4Print (float a[]) {
    Mat4fPrint (stdout, a);
}

void Mat4fPrint (FILE *fd, float a[]) {
    int i;
    if (fd == NULL || a == NULL) return;

    for (i = 0; i < 16; ++i) {
            fprintf(fd,"%.2f %s",a[i],((i+1) % 4 ==0 ? "\n" : ""));
    }
}

void Mat4Vec4Mult (float m[], float v[], float ret[]) {
    float temp[4] = {0.0f};

    if (m == NULL || v == NULL || ret == NULL) return;

    gsl_vector_float_view V = gsl_vector_float_view_array(v,4);
    gsl_vector_float_view R = gsl_vector_float_view_array(ret,4);
    gsl_vector_float_view T = gsl_vector_float_view_array(temp,4);
    gsl_matrix_float_view M = gsl_matrix_float_view_array(m,4,4);

    gsl_blas_sgemv(CblasNoTrans, 1.0f, &M.matrix, &V.vector, 0.0f, &T.vector);
    gsl_vector_float_memcpy(&R.vector, &T.vector);
}

void Vec4fPrint(FILE *fd, float v[]) {
    int i;
    if (fd == NULL || v == NULL) return;

    for (i = 0; i < 4; ++i) {
            fprintf(fd, "%.2f ", v[i]);
    }
    fprintf(fd,"\n");
}

void Vec4Print(float v[]) {
    Vec4fPrint(stdout, v);
}

void Vec3Copy (float dst[], float src[]) {
    int i;
    if (dst == NULL || src == NULL) return;
    
    for (i = 0; i < 3; ++i) {
        dst[i] = src[i];
    }
}

float Vec3Dot (float a[], float b[]) {
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

// found this at:
// http://stackoverflow.com/questions/4436764/rotating-a-quaternion-on-1-axis
void QuaternionFromAxisAngle(float x, float y, float z, float a, float out[4]) {
    if (out == NULL) return;
    
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
    if (m == NULL || n == NULL || out == NULL) return;
    
    float temp[4] = {0.0f};
    temp[0] = (m[3] * n[0]) + (m[0] * n[3]) + (m[1] * n[2]) - (m[2] * n[1]);
    temp[1] = (m[3] * n[1]) - (m[0] * n[2]) + (m[1] * n[3]) + (m[2] * n[0]);
    temp[2] = (m[3] * n[2]) + (m[0] * n[1]) - (m[1] * n[0]) + (m[2] * n[3]);
    temp[3] = (m[3] * n[3]) - (m[0] * n[0]) - (m[1] * n[1]) - (m[2] * n[2]);
    
    for (int i = 0; i < 4; ++i) {
        out[i] = temp[i];
    }
}