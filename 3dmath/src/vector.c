#include <assert.h>
#include <math.h>
#include "vector.h"

static_assert(sizeof(TmVec2) == sizeof(TmScalar[2]), "TmVec2 has unexpected padding");
static_assert(sizeof(TmVec3) == sizeof(TmScalar[3]), "TmVec3 has unexpected padding");
static_assert(sizeof(TmVec4) == sizeof(TmScalar[4]), "TmVec4 has unexpected padding");

TmVec2 *
tmVec2Add(TmVec2 *dest, TmVec2 const *p, TmVec2 const *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;

	return dest;
}

TmScalar
tmVec2Distance(TmVec2 const *p, TmVec2 const *q)
{
	TmVec2 displacement;
	TmScalar magnitudeOfDisplacement;

	tmVec2Sub(&displacement, p, q);
	magnitudeOfDisplacement = tmVec2Length(&displacement);

	return magnitudeOfDisplacement;
}

TmScalar
tmVec2Dot(TmVec2 const *p, TmVec2 const *q)
{
	TmScalar result;

	result = ((p->x * q->x) + (p->y * q->y));

	return result;
}

TmScalar
tmVec2Length(TmVec2 const *p)
{
	TmScalar squaredLength;
	TmScalar length;

	squaredLength = tmVec2SqLength(p);
	length = tmScalarSqrt(squaredLength);

	return length;
}

TmVec2 *
tmVec2Normalize(TmVec2 *dest, TmVec2 const *p)
{
	TmScalar length;

	length = tmVec2Length(p);
	assert(length != 0.0f);
	dest->x = p->x / length;
	dest->y = p->y / length;

	return dest;
}

TmVec2 *
tmVec2Projection(TmVec2 *pOntoQ, TmVec2 const *p, TmVec2 const *q)
{
	TmVec2 temp;
	TmScalar pDotQ, qLenSquared;

	pDotQ = tmVec2Dot(p, q);
	qLenSquared = tmVec2SqLength(q);
	tmVec2Scale(&temp, pDotQ / qLenSquared, q);
	*pOntoQ = temp;

	return pOntoQ;
}

TmVec2 *
tmVec2Scale(TmVec2 *dest, TmScalar scale, TmVec2 const *p)
{
	dest->x = scale * p->x;
	dest->y = scale * p->y;

	return dest;
}

TmScalar
tmVec2SqLength(TmVec2 const *p)
{
	TmScalar squaredLength;

	squaredLength = ((p->x * p->x) + (p->y * p->y));

	return squaredLength;
}

TmVec2 *
tmVec2Sub(TmVec2 *dest, TmVec2 const *p, TmVec2 const *q)
{
	dest->x = p->x - q->x;
	dest->y = p->y - q->y;

	return dest;
}

TmVec3 *
tmVec3Add(TmVec3 *dest, TmVec3 const *p, TmVec3 const *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;
	dest->z = p->z + q->z;

	return dest;
}

TmVec3 *
tmVec3Cross(TmVec3 *dest, TmVec3 const *p, TmVec3 const *q)
{
	dest->x = (p->y * q->z) - (p->z * q->y);
	dest->y = (p->z * q->x) - (p->x * q->z);
	dest->z = (p->x * q->y) - (p->y * q->x);

	return dest;
}

TmScalar
tmVec3Distance(TmVec3 const *p, TmVec3 const *q)
{
	TmVec3 displacement;
	TmScalar magnitudeOfDisplacement;

	tmVec3Sub(&displacement, p, q);
	magnitudeOfDisplacement = tmVec3Length(&displacement);

	return magnitudeOfDisplacement;
}

TmScalar
tmVec3Dot(TmVec3 const *p, TmVec3 const *q)
{
	TmScalar dotProduct;

	dotProduct = ((p->x * q->x) +
	              (p->y * q->y) +
	              (p->z * q->z));

	return dotProduct;
}

TmScalar
tmVec3Length(TmVec3 const *p)
{
	TmScalar squaredLength;
	TmScalar length;

	squaredLength = tmVec3SqLength(p);
	length = tmScalarSqrt(squaredLength);

	return length;
}

TmVec3 *
tmVec3Normalize(TmVec3 *dest, TmVec3 const *p)
{
	TmScalar length;

	length = tmVec3Length(p);
	assert(length != 0.0f);
	dest->x = p->x / length;
	dest->y = p->y / length;
	dest->z = p->z / length;

	return dest;
}

TmVec3 *
tmVec3Projection(TmVec3 *pOntoQ, TmVec3 const *p, TmVec3 const *q)
{
	TmVec3 temp;
	TmScalar pDotQ;
    TmScalar qLenSquared;

	pDotQ = tmVec3Dot(p, q);
	qLenSquared = tmVec3SqLength(q);
	tmVec3Scale(&temp, pDotQ / qLenSquared, q);
	*pOntoQ = temp;

	return pOntoQ;
}

TmVec3 *
tmVec3Scale(TmVec3 *dest, TmScalar scale, TmVec3 const *p)
{
	dest->x = scale * p->x;
	dest->y = scale * p->y;
	dest->z = scale * p->z;

	return dest;
}

TmScalar
tmVec3SqLength(TmVec3 const *p)
{
	TmScalar squaredLength;

	squaredLength = ((p->x * p->x) +
                     (p->y * p->y) +
                     (p->z * p->z));

	return squaredLength;
}

TmVec3 *
tmVec3Sub(TmVec3 *dest, TmVec3 const *p, TmVec3 const *q)
{
	dest->x = p->x - q->x;
	dest->y = p->y - q->y;
	dest->z = p->z - q->z;

	return dest;
}

TmVec4 *
tmVec4Add(TmVec4 *dest, TmVec4 const *p, TmVec4 const *q)
{
	dest->x = p->x + q->x;
	dest->y = p->y + q->y;
	dest->z = p->z + q->z;
	dest->w = p->w + q->w;

	return dest;
}

TmScalar
tmVec4Distance(TmVec4 const *p, TmVec4 const *q)
{
	TmVec4 displacement;
	TmScalar magnitudeOfDisplacement;

	tmVec4Sub(&displacement, p, q);
	magnitudeOfDisplacement = tmVec4Length(&displacement);

	return magnitudeOfDisplacement;
}

TmScalar
tmVec4Dot(TmVec4 const *p, TmVec4 const *q)
{
	TmScalar dotProduct;

	dotProduct = ((p->x * q->x) +
	              (p->y * q->y) +
	              (p->z * q->z) +
	              (p->w * q->w));

	return dotProduct;
}

TmScalar
tmVec4Length(TmVec4 const *p)
{
	TmScalar squaredLength;
	TmScalar length;

	squaredLength = tmVec4SqLength(p);
	length = tmScalarSqrt(squaredLength);

	return length;
}

TmVec4 *
tmVec4Normalize(TmVec4 *dest, TmVec4 const *p)
{
	TmScalar length;

	length = tmVec4Length(p);
	assert(length != 0.0f);
	dest->x = p->x / length;
	dest->y = p->y / length;
	dest->z = p->z / length;
	dest->w = p->w / length;

	return dest;
}

TmVec4 *
tmVec4Projection(TmVec4 *pOntoQ, TmVec4 const *p, TmVec4 const *q)
{
	TmVec4 temp;
	TmScalar pDotQ, qLenSquared;

	pDotQ = tmVec4Dot(p, q);
	qLenSquared = tmVec4SqLength(q);
	tmVec4Scale(&temp, pDotQ / qLenSquared, q);
	*pOntoQ = temp;

	return pOntoQ;
}

TmVec4 *
tmVec4Scale(TmVec4 *dest, TmScalar scale, TmVec4 const *p)
{
	dest->x = scale * p->x;
	dest->y = scale * p->y;
	dest->z = scale * p->z;
	dest->w = scale * p->w;

	return dest;
}

TmScalar
tmVec4SqLength(TmVec4 const *p)
{
	TmScalar squaredLength;

	squaredLength = ((p->x * p->x) +
	                 (p->y * p->y) +
	                 (p->z * p->z) +
	                 (p->w * p->w));

  	return squaredLength;
}

TmVec4 *
tmVec4Sub(TmVec4 *dest, TmVec4 const *p, TmVec4 const *q)
{
	dest->x = p->x - q->x;
	dest->y = p->y - q->y;
	dest->z = p->z - q->z;
	dest->w = p->w - q->w;

	return dest;
}

