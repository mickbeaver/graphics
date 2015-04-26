#include <assert.h>
#include <tgmath.h>
#include "vector.h"

VEC2 *
vec2_add(VEC2 *dest, const VEC2 *p, const VEC2 *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;

	return (dest);
}

scalar
vec2_dot(const VEC2 *p, const VEC2 *q)
{
	scalar result;

	result = ((p->x * q->x) + (p->y * q->y));

	return (result);
}

scalar
vec2_length(const VEC2 *p)
{
	scalar squared_length;
	scalar length;

	squared_length = vec2_sqlength(p);
	length = sqrt(squared_length);

	return (length);
}

VEC2 *
vec2_normalize(VEC2 *dest, const VEC2 *p)
{
	scalar length;

	length = vec2_length(p);
	assert(length != 0.0f);
	dest->x = p->x / length;
	dest->y = p->y / length;

	return (dest);
}

VEC2 *
vec2_scale(VEC2 *dest, scalar scale, const VEC2 *p)
{
	dest->x = scale * p->x;
	dest->y = scale * p->y;

	return (dest);
}

scalar
vec2_sqlength(const VEC2 *p)
{
	scalar squared_length;

	squared_length = ((p->x * p->x) + (p->y * p->y));

	return (squared_length);
}

VEC3 *
vec3_add(VEC3 *dest, const VEC3 *p, const VEC3 *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;
	dest->z = p->z + q->z;

	return (dest);
}

VEC3 *
vec3_cross(VEC3 *dest, const VEC3 *p, const VEC3 *q)
{
	VEC3 temp;

	temp.x = (p->y * q->z) - (p->z * q->y);
	temp.y = (p->z * q->x) - (p->x * q->z);
	temp.z = (p->x * q->y) - (p->y * q->x);

	*dest = temp;

	return (dest);
}

scalar
vec3_dot(const VEC3 *p, const VEC3 *q)
{
	scalar dot_product;

	dot_product = ((p->x * q->x) +
	    (p->y * q->y) +
	    (p->z * q->z));

	return (dot_product);
}

scalar
vec3_length(const VEC3 *p)
{
	scalar squared_length;
	scalar length;

	squared_length = vec3_sqlength(p);
	length = sqrt(squared_length);

	return (length);
}

VEC3 *
vec3_normalize(VEC3 *dest, const VEC3 *p)
{
	scalar length;

	length = vec3_length(p);
	assert(length != 0.0f);
	dest->x = p->x / length;
	dest->y = p->y / length;
	dest->z = p->z / length;

	return (dest);
}

VEC3 *
vec3_scale(VEC3 *dest, scalar scale, const VEC3 *p)
{
	dest->x = scale * p->x;
	dest->y = scale * p->y;
	dest->z = scale * p->z;

	return (dest);
}

scalar
vec3_sqlength(const VEC3 *p)
{
	scalar squared_length;

	squared_length = ((p->x * p->x) + (p->y * p->y) + (p->z * p->z));

	return (squared_length);
}

VEC4 *
vec4_add(VEC4 *dest, const VEC4 *p, const VEC4 *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;
	dest->z = p->z + q->z;
	dest->w = p->w + q->w;

	return (dest);
}

scalar
vec4_dot(const VEC4 *p, const VEC4 *q)
{
	scalar dot_product;

	dot_product = ((p->x * q->x) +
	    (p->y * q->y) +
	    (p->z * q->z) +
	    (p->w * q->w));

	return (dot_product);
}

scalar
vec4_length(const VEC4 *p)
{
	scalar squared_length;
	scalar length;

	squared_length = vec4_sqlength(p);
	length = sqrt(squared_length);

	return (length);
}

VEC4 *
vec4_normalize(VEC4 *dest, const VEC4 *p)
{
	scalar length;

	length = vec4_length(p);
	assert(length != 0.0f);
	dest->x = p->x / length;
	dest->y = p->y / length;
	dest->z = p->z / length;
	dest->w = p->w / length;

	return (dest);
}

VEC4 *
vec4_scale(VEC4 *dest, scalar scale, const VEC4 *p)
{
	dest->x = scale * p->x;
	dest->y = scale * p->y;
	dest->z = scale * p->z;
	dest->w = scale * p->w;

	return (dest);
}

scalar
vec4_sqlength(const VEC4 *p)
{
	scalar squared_length;

	squared_length = ((p->x * p->x) +
	    (p->y * p->y) +
	    (p->z * p->z) +
	    (p->w * p->w));

	return (squared_length);
}

