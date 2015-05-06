#include "matrix.h"

MAT2 *
mat2_add(MAT2 *dest, const MAT2 *a, const MAT2 *b)
{
	dest->m11 = a->m11 + b->m11;
	dest->m12 = a->m12 + b->m12;
	dest->m21 = a->m21 + b->m21;
	dest->m22 = a->m22 + b->m22;

	return (dest);
}

MAT2 *
mat2_multiply(MAT2 *dest, const MAT2 *a, const MAT2 *b)
{
	MAT2 result;

	result.m11 = (a->m11 * b->m11) + (a->m12 * b->m21);
	result.m12 = (a->m11 * b->m12) + (a->m12 * b->m22);
	result.m21 = (a->m21 * b->m11) + (a->m22 * b->m21);
	result.m22 = (a->m21 * b->m12) + (a->m22 * b->m22);
	*dest = result;

	return (dest);
}

