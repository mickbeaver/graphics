#ifndef GRAPHICS_MATH_MATRIX_H
#define GRAPHICS_MATH_MATRIX_H

#include "scalar.h"

typedef struct mat2 {
	scalar m11, m21;
	scalar m12, m22;
} MAT2;

typedef struct mat3 {
	scalar m11, m21, m31;
	scalar m12, m22, m32;
	scalar m13, m23, m33;
} MAT3;

typedef struct mat4 {
	scalar m11, m21, m31, m41;
	scalar m12, m22, m32, m42;
	scalar m13, m23, m33, m43;
	scalar m14, m24, m34, m44;
} MAT4;

MAT2	*mat2_add(MAT2 *dest, const MAT2 *a, const MAT2 *b);
MAT2	*mat2_multiply(MAT2 *dest, const MAT2 *a, const MAT2 *b);
MAT2	*mat2_transpose(MAT2 *dest, const MAT2 *a);
scalar	 mat2_determinant(const MAT2 *a);
MAT2	*mat2_inverse(MAT2 *dest, const MAT2 *a);

#endif /* GRAPHICS_MATH_MATRIX_H */
