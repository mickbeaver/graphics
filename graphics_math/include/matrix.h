#ifndef GRAPHICS_MATH_MATRIX_H
#define GRAPHICS_MATH_MATRIX_H

#include "scalar.h"

typedef struct mat2 {
	scalar m11;
	scalar m12;
	scalar m21;
	scalar m22;
} MAT2;

MAT2	*mat2_add(MAT2 *dest, const MAT2 *a, const MAT2 *b);
MAT2	*mat2_multiply(MAT2 *dest, const MAT2 *a, const MAT2 *b);

#endif /* GRAPHICS_MATH_MATRIX_H */
