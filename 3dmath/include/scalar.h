#ifndef GRAPHICS_MATH_SCALAR_H
#define GRAPHICS_MATH_SCALAR_H

#define TM_SCALAR_TYPE_FLOAT 0x2
#define TM_SCALAR_TYPE_DOUBLE 0x4 
#define TM_SCALAR_TYPE_LONG_DOUBLE 0x8

#define TM_SCALAR_TYPE TM_SCALAR_TYPE_FLOAT

#if TM_SCALAR_TYPE == TM_SCALAR_TYPE_FLOAT
typedef float TmScalar;

#define TM_SCALAR_PI 3.14159265358979323846f

// <tgmath.h> should do the right thing, but does not work correctly on
// FreeBSD's headers. These defines should be updated if the scalar
// typedef changes.
#define tmScalarFabs fabsf
#define tmScalarSqrt sqrtf
#define tmScalarTan  tanf
#define tmScalarCos  cosf
#define tmScalarSin  sinf

TmScalar    tmScalarMix(TmScalar x, TmScalar y, TmScalar a);

#else
#error "TM_SCALAR_TYPE set to an invalid value"
#endif

#endif /* GRAPHICS_MATH_SCALAR_H */
