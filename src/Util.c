#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <gsl/gsl_blas.h>

#include "Util.h"

float degreesToRadians (float d) {
	return d / (180.0f * M_PI);
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
	a[3] = v[0];
	a[7] = v[1];
	a[11] = v[2];
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
    
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[3]);
}