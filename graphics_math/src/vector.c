#include "vector.h"

VEC2 *
vec2_add(VEC2 *dest, const VEC2 *a, const VEC2 *b)
{
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	return (dest);
}

