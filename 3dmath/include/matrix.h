#ifndef GRAPHICS_MATH_MATRIX_H
#define GRAPHICS_MATH_MATRIX_H

#include "scalar.h"
#include "vector.h"

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
scalar	 mat2_determinant(const MAT2 *a);
MAT2	*mat2_inverse(MAT2 *dest, const MAT2 *a);
MAT2	*mat2_multiply(MAT2 *dest, const MAT2 *a, const MAT2 *b);
MAT2	*mat2_scalar_multiply(MAT2 *dest, const MAT2 *a, scalar x);
MAT2	*mat2_transpose(MAT2 *dest, const MAT2 *a);

MAT3	*mat3_add(MAT3 *dest, const MAT3 *a, const MAT3 *b);
scalar	 mat3_determinant(const MAT3 *a);
MAT3	*mat3_inverse(MAT3 *dest, const MAT3 *a);
MAT3	*mat3_multiply(MAT3 *dest, const MAT3 *a, const MAT3 *b);
MAT3	*mat3_scalar_multiply(MAT3 *dest, const MAT3 *a, scalar x);
MAT3	*mat3_transpose(MAT3 *dest, const MAT3 *a);

MAT4	*mat4_add(MAT4 *dest, const MAT4 *a, const MAT4 *b);
scalar	 mat4_determinant(const MAT4 *a);
MAT4	*mat4_identity(MAT4 *dest);
MAT4	*mat4_inverse(MAT4 *dest, const MAT4 *a);
MAT4	*mat4_multiply(MAT4 *dest, const MAT4 *a, const MAT4 *b);
MAT4	*mat4_rotation(MAT4 *dest, const VEC3 *normal, scalar angle);
MAT4	*mat4_scalar_multiply(MAT4 *dest, const MAT4 *a, scalar x);
MAT4	*mat4_transpose(MAT4 *dest, const MAT4 *a);
MAT4	*mat4_look_at(MAT4 *dest, const VEC3 *eye, const VEC3 *center, const VEC3 *up);
MAT4	*mat4_perspective(MAT4 *dest, scalar fov_y, scalar aspect_ratio, scalar z_near, scalar z_far);
#endif /* GRAPHICS_MATH_MATRIX_H */
