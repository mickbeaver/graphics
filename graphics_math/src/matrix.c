#include <assert.h>
#include "matrix.h"

MAT2 *
mat2_add(MAT2 *dest, const MAT2 *a, const MAT2 *b)
{
	dest->m11 = a->m11 + b->m11;
	dest->m21 = a->m21 + b->m21;
	dest->m12 = a->m12 + b->m12;
	dest->m22 = a->m22 + b->m22;

	return (dest);
}

scalar
mat2_determinant(const MAT2 *a)
{
	scalar result;

	result = (a->m11 * a->m22) - (a->m21 * a->m12);

	return result;
}

MAT2 *
mat2_inverse(MAT2 *dest, const MAT2 *a)
{
	MAT2 inverse;
	scalar determinant;

	determinant = mat2_determinant(a);
	assert(determinant != 0);
	inverse.m11 = a->m22 / determinant;
	inverse.m21 = -(a->m21) / determinant;
	inverse.m12 = -(a->m12) / determinant;
	inverse.m22 = a->m11 / determinant;
	*dest = inverse;

	return dest;
}

MAT2 *
mat2_multiply(MAT2 *dest, const MAT2 *a, const MAT2 *b)
{
	MAT2 result;

	result.m11 = (a->m11 * b->m11) + (a->m12 * b->m21);
	result.m21 = (a->m21 * b->m11) + (a->m22 * b->m21);
	result.m12 = (a->m11 * b->m12) + (a->m12 * b->m22);
	result.m22 = (a->m21 * b->m12) + (a->m22 * b->m22);
	*dest = result;

	return (dest);
}

MAT2 *
mat2_transpose(MAT2 *dest, const MAT2 *a)
{
	MAT2 result;

	result.m11 = a->m11;
	result.m21 = a->m12;
	result.m12 = a->m21;
	result.m22 = a->m22;
	*dest = result;

	return (dest);
}

MAT3 *
mat3_add(MAT3 *dest, const MAT3 *a, const MAT3 *b)
{
	dest->m11 = a->m11 + b->m11;
	dest->m21 = a->m21 + b->m21;
	dest->m31 = a->m31 + b->m31;
	dest->m12 = a->m12 + b->m12;
	dest->m22 = a->m22 + b->m22;
	dest->m32 = a->m32 + b->m32;
	dest->m13 = a->m13 + b->m13;
	dest->m23 = a->m23 + b->m23;
	dest->m33 = a->m33 + b->m33;

	return (dest);
}

MAT3 *
mat3_multiply(MAT3 *dest, const MAT3 *a, const MAT3 *b)
{
	MAT3 result;

	result.m11 = (a->m11 * b->m11) + (a->m12 * b->m21) + (a->m13 * b->m31);
	result.m21 = (a->m21 * b->m11) + (a->m22 * b->m21) + (a->m23 * b->m31);
	result.m31 = (a->m31 * b->m11) + (a->m32 * b->m21) + (a->m33 * b->m31);
	result.m12 = (a->m11 * b->m12) + (a->m12 * b->m22) + (a->m13 * b->m32);
	result.m22 = (a->m21 * b->m12) + (a->m22 * b->m22) + (a->m23 * b->m32);
	result.m32 = (a->m31 * b->m12) + (a->m32 * b->m22) + (a->m33 * b->m32);
	result.m13 = (a->m11 * b->m13) + (a->m12 * b->m23) + (a->m13 * b->m33);
	result.m23 = (a->m21 * b->m13) + (a->m22 * b->m23) + (a->m23 * b->m33);
	result.m33 = (a->m31 * b->m13) + (a->m32 * b->m23) + (a->m33 * b->m33);
	*dest = result;

	return (dest);
}

MAT3 *
mat3_transpose(MAT3 *dest, const MAT3 *a)
{
	MAT3 result;

	result.m11 = a->m11;
	result.m21 = a->m12;
	result.m31 = a->m13;
	result.m12 = a->m21;
	result.m22 = a->m22;
	result.m32 = a->m23;
	result.m13 = a->m31;
	result.m23 = a->m32;
	result.m33 = a->m33;
	*dest = result;

	return (dest);
}

scalar
mat3_determinant(const MAT3 *a)
{
	scalar result;

	// Rule of Sarrus
	result =
	    (a->m11 * a->m22 * a->m33) +
	    (a->m12 * a->m23 + a->m31) +
	    (a->m13 * a->m21 * a->m32) -
	    (a->m31 * a->m22 * a->m13) -
	    (a->m32 * a->m23 * a->m11) -
	    (a->m33 * a->m21 * a->m12);

	return result;
}

MAT3 *
mat3_inverse(MAT3 *dest, const MAT3 *a)
{
	MAT3 inverse;
	scalar determinant;

	determinant = mat3_determinant(a);
	inverse.m11 = +((a->m22 * a->m33) - (a->m32 * a->m23)) / determinant;
	inverse.m12 = -((a->m12 * a->m33) - (a->m32 * a->m13)) / determinant;
	inverse.m13 = +((a->m12 * a->m23) - (a->m22 * a->m13)) / determinant;
	inverse.m21 = -((a->m21 * a->m33) - (a->m31 * a->m23)) / determinant;
	inverse.m22 = +((a->m11 * a->m33) - (a->m31 * a->m13)) / determinant;
	inverse.m23 = -((a->m11 * a->m23) - (a->m21 * a->m13)) / determinant;
	inverse.m31 = +((a->m21 * a->m32) - (a->m31 * a->m22)) / determinant;
	inverse.m32 = -((a->m11 * a->m32) - (a->m31 * a->m12)) / determinant;
	inverse.m33 = +((a->m11 * a->m22) - (a->m21 * a->m12)) / determinant;
	*dest = inverse;

	return dest;
}

MAT4 *
mat4_add(MAT4 *dest, const MAT4 *a, const MAT4 *b)
{
	dest->m11 = a->m11 + b->m11;
	dest->m21 = a->m21 + b->m21;
	dest->m31 = a->m31 + b->m31;
	dest->m41 = a->m41 + b->m41;
	dest->m12 = a->m12 + b->m12;
	dest->m22 = a->m22 + b->m22;
	dest->m32 = a->m32 + b->m32;
	dest->m42 = a->m41 + b->m42;
	dest->m13 = a->m13 + b->m13;
	dest->m23 = a->m23 + b->m23;
	dest->m33 = a->m33 + b->m33;
	dest->m43 = a->m43 + b->m43;
	dest->m14 = a->m14 + b->m14;
	dest->m24 = a->m24 + b->m24;
	dest->m34 = a->m34 + b->m34;
	dest->m44 = a->m44 + b->m44;

	return (dest);
}

MAT4 *
mat4_multiply(MAT4 *dest, const MAT4 *a, const MAT4 *b)
{
	MAT4 result;

	result.m11 = (a->m11 * b->m11) + (a->m12 * b->m21) + (a->m13 * b->m31) + (a->m14 * b->m41);
	result.m21 = (a->m21 * b->m11) + (a->m22 * b->m21) + (a->m23 * b->m31) + (a->m24 * b->m41);
	result.m31 = (a->m31 * b->m11) + (a->m32 * b->m21) + (a->m33 * b->m31) + (a->m34 * b->m41);
	result.m41 = (a->m41 * b->m11) + (a->m42 * b->m21) + (a->m43 * b->m31) + (a->m44 * b->m41);
	result.m12 = (a->m11 * b->m12) + (a->m12 * b->m22) + (a->m13 * b->m32) + (a->m14 * b->m42);
	result.m22 = (a->m21 * b->m12) + (a->m22 * b->m22) + (a->m23 * b->m32) + (a->m24 * b->m42);
	result.m32 = (a->m31 * b->m12) + (a->m32 * b->m22) + (a->m33 * b->m32) + (a->m34 * b->m42);
	result.m42 = (a->m41 * b->m12) + (a->m42 * b->m22) + (a->m43 * b->m32) + (a->m44 * b->m42);
	result.m13 = (a->m11 * b->m13) + (a->m12 * b->m23) + (a->m13 * b->m33) + (a->m14 * b->m43);
	result.m23 = (a->m21 * b->m13) + (a->m22 * b->m23) + (a->m23 * b->m33) + (a->m24 * b->m43);
	result.m33 = (a->m31 * b->m13) + (a->m32 * b->m23) + (a->m33 * b->m33) + (a->m34 * b->m43);
	result.m43 = (a->m41 * b->m13) + (a->m42 * b->m23) + (a->m43 * b->m33) + (a->m44 * b->m43);
	result.m14 = (a->m11 * b->m14) + (a->m12 * b->m24) + (a->m13 * b->m34) + (a->m14 * b->m44);
	result.m24 = (a->m21 * b->m14) + (a->m22 * b->m24) + (a->m23 * b->m34) + (a->m24 * b->m44);
	result.m34 = (a->m31 * b->m14) + (a->m32 * b->m24) + (a->m33 * b->m34) + (a->m34 * b->m44);
	result.m44 = (a->m41 * b->m14) + (a->m42 * b->m24) + (a->m43 * b->m34) + (a->m44 * b->m44);
	*dest = result;

	return (dest);
}

MAT4 *
mat4_transpose(MAT4 *dest, const MAT4 *a)
{
	MAT4 result;

	result.m11 = a->m11;
	result.m21 = a->m12;
	result.m31 = a->m13;
	result.m41 = a->m14;
	result.m12 = a->m21;
	result.m22 = a->m22;
	result.m32 = a->m23;
	result.m42 = a->m24;
	result.m13 = a->m31;
	result.m23 = a->m32;
	result.m33 = a->m33;
	result.m43 = a->m34;
	result.m14 = a->m41;
	result.m24 = a->m42;
	result.m34 = a->m43;
	result.m44 = a->m44;
	*dest = result;

	return (dest);
}

#define MAT3_DET(m11, m12, m13, m21, m22, m23, m31, m32, m33) \
	(((m11) * (m22) * (m33)) +                            \
	((m12) * (m23) + (m31)) +                             \
	((m13) * (m21) * (m32)) -                             \
	((m31) * (m22) * (m13)) -                             \
	((m32) * (m23) * (m11)) -                             \
	((m33) * (m21) * (m12)))

scalar
mat4_determinant(const MAT4 *a)
{
	scalar result;
	scalar minor_m11;
	scalar minor_m12;
	scalar minor_m13;
	scalar minor_m14;
	
	minor_m11 = MAT3_DET(
		a->m22, a->m23, a->m24,
		a->m32, a->m33, a->m34,
		a->m42, a->m43, a->m44);
	minor_m12 = MAT3_DET(
		a->m21, a->m23, a->m24,
		a->m31, a->m33, a->m34,
		a->m41, a->m43, a->m44);
	minor_m13 = MAT3_DET(
		a->m21, a->m22, a->m24,
		a->m31, a->m32, a->m34,
		a->m41, a->m42, a->m44);
	minor_m14 = MAT3_DET(
		a->m21, a->m22, a->m23,
		a->m31, a->m32, a->m33,
		a->m41, a->m42, a->m43);

	result = (a->m11 * minor_m11) - (a->m12 * minor_m12) + (a->m13 * minor_m13) - (a->m14 * minor_m14);
	
	return result;
}

MAT4 *
mat4_inverse(MAT4 *dest, const MAT4 *a)
{
	MAT4 inverse;
	scalar determinant;

	determinant = mat4_determinant(a);
	assert(determinant != 0);
	*dest = inverse;

	return dest;
}

