#ifndef GRAPHICS_MATH_SCALAR_H
#define GRAPHICS_MATH_SCALAR_H

#define SCALAR_TYPE_FLOAT 0x2
#define SCALAR_TYPE_DOUBLE 0x4 
#define SCALAR_TYPE_LONG_DOUBLE 0x8

#define SCALAR_TYPE SCALAR_TYPE_FLOAT

#if SCALAR_TYPE == SCALAR_TYPE_FLOAT
typedef float TmScalar;

#define SCALAR_PI 3.14159265358979323846f

// <tgmath.h> should do the right thing, but does not work correctly on
// FreeBSD's headers. These defines should be updated if the scalar
// typedef changes.
#define tmScalarFabs fabsf
#define tmScalarSqrt sqrtf
#define tmScalarTan  tanf
#define tmScalarCos  cosf
#define tmScalarSin  sinf

#else
#error "SCALAR_TYPE set to an invalid value"
#endif

#endif /* GRAPHICS_MATH_SCALAR_H */
