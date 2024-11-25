/* matrix.h */

#ifndef MATRIX_H
#define MATRIX_H


#define M_PI 3.1415926536f

typedef struct
{
	float x;
	float y;
	float z;
}vec3;

extern float normalise(vec3* v);
extern void rotate(float angle, vec3 v, float* pMatrix);
extern void matrixMultiply(float* matrix1, float* matrix2, float* matrix3);


#endif

