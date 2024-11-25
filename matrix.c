/* matrix.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "matrix.h"

float normalise(vec3* v)
{
	float mag = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);

	if (mag)
	{
		float divmag = 1.0f / mag;
		v->x *= divmag;
		v->y *= divmag;
		v->z *= divmag;
	}

	return mag;
}

void rotate(float angle, vec3 v, float* pMatrix)
{
	float c, s;
	angle = angle / 180.0f * (float)M_PI;

	normalise(&v);

	c = 1.0f - cosf(angle);
	s = sinf(angle);

	pMatrix[0] = 1.0f + c * (v.x * v.x - 1.0f);
	pMatrix[1] = c * v.x * v.y + v.z * s;
	pMatrix[2] = c * v.x * v.z - v.y * s;

	pMatrix[3] = c * v.x * v.y - v.z * s;
	pMatrix[4] = 1.0f + c * (v.y * v.y - 1.0f);
	pMatrix[5] = c * v.y * v.z + v.x * s;

	pMatrix[6] = c * v.x * v.z + v.y * s;
	pMatrix[7] = c * v.y * v.z - v.x * s;
	pMatrix[8] = 1.0f + c * (v.z * v.z - 1.0f);
}

void matrixMultiply(float* matrix1, float* matrix2, float* matrix3)
{
	matrix3[0] = matrix1[0] * matrix2[0] + matrix1[3] * matrix2[1] + matrix1[6] * matrix2[2];
	matrix3[1] = matrix1[1] * matrix2[0] + matrix1[4] * matrix2[1] + matrix1[7] * matrix2[2];
	matrix3[2] = matrix1[2] * matrix2[0] + matrix1[5] * matrix2[1] + matrix1[8] * matrix2[2];
	matrix3[3] = matrix1[0] * matrix2[3] + matrix1[3] * matrix2[4] + matrix1[6] * matrix2[5];
	matrix3[4] = matrix1[1] * matrix2[3] + matrix1[4] * matrix2[4] + matrix1[7] * matrix2[5];
	matrix3[5] = matrix1[2] * matrix2[3] + matrix1[5] * matrix2[4] + matrix1[8] * matrix2[5];
	matrix3[6] = matrix1[0] * matrix2[6] + matrix1[3] * matrix2[7] + matrix1[6] * matrix2[8];
	matrix3[7] = matrix1[1] * matrix2[6] + matrix1[4] * matrix2[7] + matrix1[7] * matrix2[8];
	matrix3[8] = matrix1[2] * matrix2[6] + matrix1[5] * matrix2[7] + matrix1[8] * matrix2[8];
}





