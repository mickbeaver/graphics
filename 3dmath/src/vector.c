#include <assert.h>
#include <math.h>
#include "vector.h"

VEC2 *
vec2_add(VEC2 *dest, const VEC2 *p, const VEC2 *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;

	return (dest);
}

scalar
vec2_distance(const VEC2 *p, const VEC2 *q)
{
	VEC2 displacement;
	scalar magnitude_of_displacement;

	vec2_sub(&displacement, p, q);
	magnitude_of_displacement = vec2_length(&displacement);

	return (magnitude_of_displacement);
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
	length = scalar_sqrt(squared_length);

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
vec2_projection(VEC2 *p_onto_q, const VEC2 *p, const VEC2 *q)
{
	VEC2 temp;
	scalar p_dot_q, q_len_squared;

	p_dot_q = vec2_dot(p, q);
	q_len_squared = vec2_sqlength(q);
	vec2_scale(&temp, p_dot_q / q_len_squared, q);
	*p_onto_q = temp;

	return (p_onto_q);
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

VEC2 *
vec2_sub(VEC2 *dest, const VEC2 *p, const VEC2 *q)
{
	dest->x = p->x - q->x;
	dest->y = p->y - q->y;

	return (dest);
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
	dest->x = (p->y * q->z) - (p->z * q->y);
	dest->y = (p->z * q->x) - (p->x * q->z);
	dest->z = (p->x * q->y) - (p->y * q->x);

	return (dest);
}

scalar
vec3_distance(const VEC3 *p, const VEC3 *q)
{
	VEC3 displacement;
	scalar magnitude_of_displacement;

	vec3_sub(&displacement, p, q);
	magnitude_of_displacement = vec3_length(&displacement);

	return (magnitude_of_displacement);
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
	length = scalar_sqrt(squared_length);

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
vec3_projection(VEC3 *p_onto_q, const VEC3 *p, const VEC3 *q)
{
	VEC3 temp;
	scalar p_dot_q, q_len_squared;

	p_dot_q = vec3_dot(p, q);
	q_len_squared = vec3_sqlength(q);
	vec3_scale(&temp, p_dot_q / q_len_squared, q);
	*p_onto_q = temp;

	return (p_onto_q);
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

VEC3 *
vec3_sub(VEC3 *dest, const VEC3 *p, const VEC3 *q)
{
	dest->x = p->x - q->x;
	dest->y = p->y - q->y;
	dest->z = p->z - q->z;

	return (dest);
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
vec4_distance(const VEC4 *p, const VEC4 *q)
{
	VEC4 displacement;
	scalar magnitude_of_displacement;

	vec4_sub(&displacement, p, q);
	magnitude_of_displacement = vec4_length(&displacement);

	return (magnitude_of_displacement);
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
	length = scalar_sqrt(squared_length);

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
vec4_projection(VEC4 *p_onto_q, const VEC4 *p, const VEC4 *q)
{
	VEC4 temp;
	scalar p_dot_q, q_len_squared;

	p_dot_q = vec4_dot(p, q);
	q_len_squared = vec4_sqlength(q);
	vec4_scale(&temp, p_dot_q / q_len_squared, q);
	*p_onto_q = temp;

	return (p_onto_q);
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

VEC4 *
vec4_sub(VEC4 *dest, const VEC4 *p, const VEC4 *q)
{
	dest->x = p->x - q->x;
	dest->y = p->y - q->y;
	dest->z = p->z - q->z;
	dest->w = p->w - q->w;

	return (dest);
}

