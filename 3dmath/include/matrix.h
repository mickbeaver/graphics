#ifndef GRAPHICS_MATH_MATRIX_H
#define GRAPHICS_MATH_MATRIX_H

#include "scalar.h"
#include "vector.h"

// Matrices are in column-major order

typedef struct TmMat2 {
    // column 1
    scalar m11;
    scalar m21;
    // column 2
    scalar m12;
    scalar m22;
} TmMat2;

typedef struct TmMat3 {
    // column 1
    scalar m11;
    scalar m21;
    scalar m31;
    // column 2
    scalar m12;
    scalar m22;
    scalar m32;
    // column 3
    scalar m13;
    scalar m23;
    scalar m33;
} TmMat3;

typedef struct TmMat4 {
    // column 1
    scalar m11;
    scalar m21;
    scalar m31;
    scalar m41;
    // column 2
    scalar m12;
    scalar m22;
    scalar m32;
    scalar m42;
    // column 3
    scalar m13;
    scalar m23;
    scalar m33;
    scalar m43;
    // column 4
    scalar m14;
    scalar m24;
    scalar m34;
    scalar m44;
} TmMat4;

TmMat2 *tmMat2Add(TmMat2 *dest, TmMat2 const *a, TmMat2 const *b);
scalar  tmMat2Determinant(TmMat2 const *a);
TmMat2 *tmMat2Inverse(TmMat2 *dest, TmMat2 const *a);
TmMat2 *tmMat2Multiply(TmMat2 *dest, TmMat2 const *a, TmMat2 const *b);
TmMat2 *tmMat2ScalarMultiply(TmMat2 *dest, TmMat2 const *a, scalar x);
TmMat2 *tmMat2Transpose(TmMat2 *dest, TmMat2 const *a);

TmMat3 *tmMat3Add(TmMat3 *dest, TmMat3 const *a, TmMat3 const *b);
scalar  tmMat3Determinant(TmMat3 const *a);
TmMat3 *tmMat3Inverse(TmMat3 *dest, TmMat3 const *a);
TmMat3 *tmMat3Multiply(TmMat3 *dest, TmMat3 const *a, TmMat3 const *b);
TmMat3 *tmMat3ScalarMultiply(TmMat3 *dest, TmMat3 const *a, scalar x);
TmMat3 *tmMat3Transpose(TmMat3 *dest, TmMat3 const *a);

TmMat4 *tmMat4Add(TmMat4 *dest, TmMat4 const *a, TmMat4 const *b);
scalar  tmMat4Determinant(TmMat4 const *a);
TmMat4 *tmMat4Identity(TmMat4 *dest);
TmMat4 *tmMat4Inverse(TmMat4 *dest, TmMat4 const *a);
TmMat4 *tmMat4Multiply(TmMat4 *dest, TmMat4 const *a, TmMat4 const *b);
TmMat4 *tmMat4Rotation(TmMat4 *dest, const struct vec3 *normal, scalar angle);
TmMat4 *tmMat4ScalarMultiply(TmMat4 *dest, TmMat4 const *a, scalar x);
TmMat4 *tmMat4Transpose(TmMat4 *dest, TmMat4 const *a);
TmMat4 *tmMat4LookAt(TmMat4 *dest, const struct vec3 *eye, const struct vec3 *center, const struct vec3 *up);
TmMat4 *tmMat4Perspective(TmMat4 *dest, scalar fovY, scalar aspectRatio, scalar zNear, scalar zFar);

#endif /* GRAPHICS_MATH_MATRIX_H */
