#ifndef GRAPHICS_MATH_MATRIX_H
#define GRAPHICS_MATH_MATRIX_H

#include "scalar.h"
#include "vector.h"

// Matrices are in column-major order

typedef struct TmMat2 {
    // column 1
    TmScalar m11;
    TmScalar m21;
    // column 2
    TmScalar m12;
    TmScalar m22;
} TmMat2;

typedef struct TmMat3 {
    // column 1
    TmScalar m11;
    TmScalar m21;
    TmScalar m31;
    // column 2
    TmScalar m12;
    TmScalar m22;
    TmScalar m32;
    // column 3
    TmScalar m13;
    TmScalar m23;
    TmScalar m33;
} TmMat3;

typedef struct TmMat4 {
    // column 1
    TmScalar m11;
    TmScalar m21;
    TmScalar m31;
    TmScalar m41;
    // column 2
    TmScalar m12;
    TmScalar m22;
    TmScalar m32;
    TmScalar m42;
    // column 3
    TmScalar m13;
    TmScalar m23;
    TmScalar m33;
    TmScalar m43;
    // column 4
    TmScalar m14;
    TmScalar m24;
    TmScalar m34;
    TmScalar m44;
} TmMat4;

#define TM_MAT2_ZERO_INITIALIZER {.m11=0.0f, .m12=0.0f, \
                                  .m21=0.0f, .m22=0.0f}
#define TM_MAT2_ZERO ((TmMat2)TM_MAT2_ZERO_INITIALIZER)
#define TM_MAT2_IDENTITY_INITIALIZER {.m11=1.0f, .m12=0.0f, \
                                      .m21=0.0f, .m22=1.0f}
#define TM_MAT2_IDENTITY ((TmMat2)TM_MAT2_IDENTITY_INITIALIZER)
#define TM_MAT3_ZERO_INITIALIZER {.m11=0.0f, .m12=0.0f, .m13=0.0f, \
                                  .m21=0.0f, .m22=0.0f, .m23=0.0f, \
                                  .m31=0.0f, .m32=0.0f, .m33=0.0f})
#define TM_MAT3_ZERO ((TmMat3)TM_MAT3_ZERO_INITIALIZER)
#define TM_MAT3_IDENTITY_INITIALIZER {.m11=1.0f, .m12=0.0f, .m13=0.0f, \
                                      .m21=0.0f, .m22=1.0f, .m23=0.0f, \
                                      .m31=0.0f, .m32=0.0f, .m33=1.0f}
#define TM_MAT3_IDENTITY ((TmMat3)TM_MAT3_IDENTITY_INITIALIZER)
#define TM_MAT4_ZERO_INITIALIZER {.m11=0.0f, .m12=0.0f, .m13=0.0f, .m14=0.0f, \
                                  .m21=0.0f, .m22=0.0f, .m23=0.0f, .m24=0.0f, \
                                  .m31=0.0f, .m32=0.0f, .m33=0.0f, .m34=0.0f, \
                                  .m41=0.0f, .m42=0.0f, .m43=0.0f, .m44=0.0f}
#define TM_MAT4_ZERO ((TmMat4)TM_MAT4_ZERO_INITIALIZER)
#define TM_MAT4_IDENTITY_INITIALIZER {.m11=1.0f, .m12=0.0f, .m13=0.0f, .m14=0.0f, \
                                      .m21=0.0f, .m22=1.0f, .m23=0.0f, .m24=0.0f, \
                                      .m31=0.0f, .m32=0.0f, .m33=1.0f, .m34=0.0f, \
                                      .m41=0.0f, .m42=0.0f, .m43=0.0f, .m44=1.0f}
#define TM_MAT4_IDENTITY ((TmMat4)TM_MAT4_IDENTITY_INITIALIZER)

TmMat2      *tmMat2Add(TmMat2 *dest, TmMat2 const *a, TmMat2 const *b);
TmScalar     tmMat2Determinant(TmMat2 const *a);
TmMat2      *tmMat2Inverse(TmMat2 *dest, TmMat2 const *a);
TmMat2      *tmMat2Multiply(TmMat2 *dest, TmMat2 const *a, TmMat2 const *b);
TmMat2      *tmMat2ScalarMultiply(TmMat2 *dest, TmMat2 const *a, TmScalar x);
TmMat2      *tmMat2Transpose(TmMat2 *dest, TmMat2 const *a);

TmMat3      *tmMat3Add(TmMat3 *dest, TmMat3 const *a, TmMat3 const *b);
TmScalar     tmMat3Determinant(TmMat3 const *a);
TmMat3      *tmMat3Inverse(TmMat3 *dest, TmMat3 const *a);
TmMat3      *tmMat3Multiply(TmMat3 *dest, TmMat3 const *a, TmMat3 const *b);
TmMat3      *tmMat3ScalarMultiply(TmMat3 *dest, TmMat3 const *a, TmScalar x);
TmMat3      *tmMat3Transpose(TmMat3 *dest, TmMat3 const *a);

TmMat4      *tmMat4Add(TmMat4 *dest, TmMat4 const *a, TmMat4 const *b);
TmScalar     tmMat4Determinant(TmMat4 const *a);
TmMat4      *tmMat4Identity(TmMat4 *dest);
TmMat4      *tmMat4Inverse(TmMat4 *dest, TmMat4 const *a);
TmMat4      *tmMat4Multiply(TmMat4 *dest, TmMat4 const *a, TmMat4 const *b);
TmMat4      *tmMat4Rotation(TmMat4 *dest, TmVec3 const *normal, TmScalar angle);
TmMat4      *tmMat4ScalarMultiply(TmMat4 *dest, TmMat4 const *a, TmScalar x);
TmMat4      *tmMat4Transpose(TmMat4 *dest, TmMat4 const *a);
TmMat4      *tmMat4LookAt(TmMat4 *dest,
                          TmVec3 const *eye,
                          TmVec3 const *center,
                          TmVec3 const *up);
TmMat4      *tmMat4Perspective(TmMat4 *dest,
                               TmScalar fovY,
                               TmScalar aspectRatio,
                               TmScalar zNear,
                               TmScalar zFar);

#endif /* GRAPHICS_MATH_MATRIX_H */
