#include <assert.h>
#include <math.h>
#include "matrix.h"
#include "vector.h"

static_assert(sizeof(TmMat2) == sizeof(TmScalar[4]),
              "TmMat2 has unexpected padding");
static_assert(sizeof(TmMat3) == sizeof(TmScalar[9]),
              "TmMat3 has unexpected padding");
static_assert(sizeof(TmMat4) == sizeof(TmScalar[16]),
              "TmMat4 has unexpected padding");

TmMat2 *
tmMat2Add(TmMat2 *dest, TmMat2 const *a, TmMat2 const *b)
{
    dest->m11 = a->m11 + b->m11;
    dest->m21 = a->m21 + b->m21;
    dest->m12 = a->m12 + b->m12;
    dest->m22 = a->m22 + b->m22;

    return dest;
}

TmScalar
tmMat2Determinant(TmMat2 const *a)
{
    TmScalar result;

    result = (a->m11 * a->m22) - (a->m21 * a->m12);

    return result;
}

TmMat2 *
tmMat2Inverse(TmMat2 *dest, TmMat2 const *a)
{
    TmMat2 inverse;
    TmScalar determinant;

    determinant = tmMat2Determinant(a);
    assert(determinant != 0.0f);
    inverse.m11 = a->m22 / determinant;
    inverse.m21 = -(a->m21) / determinant;
    inverse.m12 = -(a->m12) / determinant;
    inverse.m22 = a->m11 / determinant;
    *dest = inverse;

    return dest;
}

TmMat2 *
tmMat2Multiply(TmMat2 *dest, TmMat2 const *a, TmMat2 const *b)
{
    dest->m11 = (a->m11 * b->m11) + (a->m12 * b->m21);
    dest->m21 = (a->m21 * b->m11) + (a->m22 * b->m21);
    dest->m12 = (a->m11 * b->m12) + (a->m12 * b->m22);
    dest->m22 = (a->m21 * b->m12) + (a->m22 * b->m22);

    return dest;
}

TmMat2 *
tmMat2ScalarMultiply(TmMat2 *dest, TmMat2 const *a, TmScalar x)
{
    dest->m11 = a->m11 * x;
    dest->m21 = a->m21 * x;
    dest->m12 = a->m12 * x;
    dest->m22 = a->m22 * x;

    return dest;
}

TmMat2 *
tmMat2Transpose(TmMat2 *dest, TmMat2 const *a)
{
    dest->m11 = a->m11;
    dest->m21 = a->m12;
    dest->m12 = a->m21;
    dest->m22 = a->m22;

    return dest;
}

TmMat3 *
tmMat3Add(TmMat3 *dest, TmMat3 const *a, TmMat3 const *b)
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

    return dest;
}

TmMat3 *
tmMat3Multiply(TmMat3 *dest, TmMat3 const *a, TmMat3 const *b)
{
    dest->m11 = (a->m11 * b->m11) + (a->m12 * b->m21) + (a->m13 * b->m31);
    dest->m21 = (a->m21 * b->m11) + (a->m22 * b->m21) + (a->m23 * b->m31);
    dest->m31 = (a->m31 * b->m11) + (a->m32 * b->m21) + (a->m33 * b->m31);
    dest->m12 = (a->m11 * b->m12) + (a->m12 * b->m22) + (a->m13 * b->m32);
    dest->m22 = (a->m21 * b->m12) + (a->m22 * b->m22) + (a->m23 * b->m32);
    dest->m32 = (a->m31 * b->m12) + (a->m32 * b->m22) + (a->m33 * b->m32);
    dest->m13 = (a->m11 * b->m13) + (a->m12 * b->m23) + (a->m13 * b->m33);
    dest->m23 = (a->m21 * b->m13) + (a->m22 * b->m23) + (a->m23 * b->m33);
    dest->m33 = (a->m31 * b->m13) + (a->m32 * b->m23) + (a->m33 * b->m33);

    return dest;
}

TmMat3 *
tmMat3ScalarMultiply(TmMat3 *dest, TmMat3 const *a, TmScalar x)
{
    dest->m11 = a->m11 * x;
    dest->m21 = a->m21 * x;
    dest->m31 = a->m31 * x;
    dest->m12 = a->m12 * x;
    dest->m22 = a->m22 * x;
    dest->m32 = a->m32 * x;
    dest->m13 = a->m13 * x;
    dest->m23 = a->m23 * x;
    dest->m33 = a->m33 * x;

    return dest;
}

TmMat3 *
tmMat3Transpose(TmMat3 *dest, TmMat3 const *a)
{
    dest->m11 = a->m11;
    dest->m21 = a->m12;
    dest->m31 = a->m13;
    dest->m12 = a->m21;
    dest->m22 = a->m22;
    dest->m32 = a->m23;
    dest->m13 = a->m31;
    dest->m23 = a->m32;
    dest->m33 = a->m33;

    return dest;
}

TmScalar
tmMat3Determinant(TmMat3 const *a)
{
    TmScalar result;

    // Rule of Sarrus
    result = ((a->m11 * a->m22 * a->m33) +
              (a->m12 * a->m23 * a->m31) +
              (a->m13 * a->m21 * a->m32) -
              (a->m31 * a->m22 * a->m13) -
              (a->m32 * a->m23 * a->m11) -
              (a->m33 * a->m21 * a->m12));

    return (result);
}

TmMat3 *
tmMat3Inverse(TmMat3 *dest, TmMat3 const *a)
{
    TmMat3 adjoint;
    TmMat3 cofactors;
    TmScalar determinant;

    determinant = tmMat3Determinant(a);
    assert(determinant != 0.0f);

    cofactors.m11 = +((a->m22 * a->m33) - (a->m32 * a->m23));
    cofactors.m12 = -((a->m21 * a->m33) - (a->m31 * a->m23));
    cofactors.m13 = +((a->m21 * a->m32) - (a->m31 * a->m22));
    cofactors.m21 = -((a->m12 * a->m33) - (a->m32 * a->m13));
    cofactors.m22 = +((a->m11 * a->m33) - (a->m31 * a->m13));
    cofactors.m23 = -((a->m11 * a->m32) - (a->m31 * a->m12));
    cofactors.m31 = +((a->m12 * a->m23) - (a->m22 * a->m13));
    cofactors.m32 = -((a->m11 * a->m23) - (a->m21 * a->m13));
    cofactors.m33 = +((a->m11 * a->m22) - (a->m21 * a->m12));

    tmMat3Transpose(&adjoint, &cofactors);
    tmMat3ScalarMultiply(dest, dest, 1.0f / determinant);

    return dest;
}

TmMat4 *
tmMat4Add(TmMat4 *dest, TmMat4 const *a, TmMat4 const *b)
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

    return dest;
}

TmMat4 *
tmMat4Multiply(TmMat4 *dest, TmMat4 const *a, TmMat4 const *b)
{
    dest->m11 = ((a->m11 * b->m11) +
                 (a->m12 * b->m21) +
                 (a->m13 * b->m31) +
                 (a->m14 * b->m41));
    dest->m21 = ((a->m21 * b->m11) +
                 (a->m22 * b->m21) +
                 (a->m23 * b->m31) +
                 (a->m24 * b->m41));
    dest->m31 = ((a->m31 * b->m11) +
                 (a->m32 * b->m21) +
                 (a->m33 * b->m31) +
                 (a->m34 * b->m41));
    dest->m41 = ((a->m41 * b->m11) +
                 (a->m42 * b->m21) +
                 (a->m43 * b->m31) +
                 (a->m44 * b->m41));
    dest->m12 = ((a->m11 * b->m12) +
                 (a->m12 * b->m22) +
                 (a->m13 * b->m32) +
                 (a->m14 * b->m42));
    dest->m22 = ((a->m21 * b->m12) +
                 (a->m22 * b->m22) +
                 (a->m23 * b->m32) +
                 (a->m24 * b->m42));
    dest->m32 = ((a->m31 * b->m12) +
                 (a->m32 * b->m22) +
                 (a->m33 * b->m32) +
                 (a->m34 * b->m42));
    dest->m42 = ((a->m41 * b->m12) +
                 (a->m42 * b->m22) +
                 (a->m43 * b->m32) +
                 (a->m44 * b->m42));
    dest->m13 = ((a->m11 * b->m13) +
                 (a->m12 * b->m23) +
                 (a->m13 * b->m33) +
                 (a->m14 * b->m43));
    dest->m23 = ((a->m21 * b->m13) +
                 (a->m22 * b->m23) +
                 (a->m23 * b->m33) +
                 (a->m24 * b->m43));
    dest->m33 = ((a->m31 * b->m13) +
                 (a->m32 * b->m23) +
                 (a->m33 * b->m33) +
                 (a->m34 * b->m43));
    dest->m43 = ((a->m41 * b->m13) +
                 (a->m42 * b->m23) +
                 (a->m43 * b->m33) +
                 (a->m44 * b->m43));
    dest->m14 = ((a->m11 * b->m14) +
                 (a->m12 * b->m24) +
                 (a->m13 * b->m34) +
                 (a->m14 * b->m44));
    dest->m24 = ((a->m21 * b->m14) +
                 (a->m22 * b->m24) +
                 (a->m23 * b->m34) +
                 (a->m24 * b->m44));
    dest->m34 = ((a->m31 * b->m14) +
                 (a->m32 * b->m24) +
                 (a->m33 * b->m34) +
                 (a->m34 * b->m44));
    dest->m44 = ((a->m41 * b->m14) +
                 (a->m42 * b->m24) +
                 (a->m43 * b->m34) +
                 (a->m44 * b->m44));

    return dest;
}

TmMat4 *
tmMat4Rotation(TmMat4 *dest, TmVec3 const *normal, TmScalar angle)
{
    TmScalar angleCos;
    TmScalar angleSin;
    TmScalar oneMinusAngleCos;
    TmScalar normalXTimesNormalY;
    TmScalar normalXTimesNormalZ;
    TmScalar normalYTimesNormalZ;

    angleCos = tmScalarCos(angle);
    angleSin = tmScalarSin(angle);
    oneMinusAngleCos = 1.0f - angleCos;
    normalXTimesNormalY = normal->x * normal->y;
    normalXTimesNormalZ = normal->x * normal->z;
    normalYTimesNormalZ = normal->y * normal->z;
    
    dest->m11 = normal->x * normal->x * oneMinusAngleCos + angleCos;
    dest->m21 = normalXTimesNormalY * oneMinusAngleCos + normal->z * angleSin;
    dest->m31 = normalXTimesNormalZ * oneMinusAngleCos - normal->y * angleSin;
    dest->m41 = 0.0f;

    dest->m12 = normalXTimesNormalY * oneMinusAngleCos - normal->z * angleSin;
    dest->m22 = normal->y * normal->y * oneMinusAngleCos + angleCos;
    dest->m32 = normalYTimesNormalZ * oneMinusAngleCos + normal->x * angleSin;
    dest->m42 = 0.0f;

    dest->m13 = normalXTimesNormalZ * oneMinusAngleCos + normal->y * angleSin;
    dest->m23 = normalYTimesNormalZ * oneMinusAngleCos - normal->x * angleSin;
    dest->m33 = normal->z * normal->z * oneMinusAngleCos + angleCos;
    dest->m43 = 0.0f;

    dest->m14 = 0.0f;
    dest->m24 = 0.0f;
    dest->m34 = 0.0f;
    dest->m44 = 1.0f;

    return dest;
}

TmMat4 *
tmMat4ScalarMultiply(TmMat4 *dest, TmMat4 const *a, TmScalar x)
{
    dest->m11 = a->m11 * x;
    dest->m21 = a->m21 * x;
    dest->m31 = a->m31 * x;
    dest->m41 = a->m41 * x;
    dest->m12 = a->m12 * x;
    dest->m22 = a->m22 * x;
    dest->m32 = a->m32 * x;
    dest->m42 = a->m42 * x;
    dest->m13 = a->m13 * x;
    dest->m23 = a->m23 * x;
    dest->m33 = a->m33 * x;
    dest->m43 = a->m43 * x;
    dest->m14 = a->m14 * x;
    dest->m24 = a->m24 * x;
    dest->m34 = a->m34 * x;
    dest->m44 = a->m44 * x;

    return dest;
}

TmMat4 *
tmMat4Transpose(TmMat4 *dest, TmMat4 const *a)
{
    dest->m11 = a->m11;
    dest->m21 = a->m12;
    dest->m31 = a->m13;
    dest->m41 = a->m14;
    dest->m12 = a->m21;
    dest->m22 = a->m22;
    dest->m32 = a->m23;
    dest->m42 = a->m24;
    dest->m13 = a->m31;
    dest->m23 = a->m32;
    dest->m33 = a->m33;
    dest->m43 = a->m34;
    dest->m14 = a->m41;
    dest->m24 = a->m42;
    dest->m34 = a->m43;
    dest->m44 = a->m44;

    return dest;
}

#define MAT3_DET(m11, m12, m13, m21, m22, m23, m31, m32, m33)   \
    (((m11) * (m22) * (m33)) +                                  \
     ((m12) * (m23) + (m31)) +                                  \
     ((m13) * (m21) * (m32)) -                                  \
     ((m31) * (m22) * (m13)) -                                  \
     ((m32) * (m23) * (m11)) -                                  \
     ((m33) * (m21) * (m12)))

TmScalar
tmMat4Determinant(TmMat4 const *a)
{
    TmScalar result;
    TmScalar minor_m11;
    TmScalar minor_m12;
    TmScalar minor_m13;
    TmScalar minor_m14;
    
    minor_m11 = MAT3_DET(a->m22, a->m23, a->m24,
                         a->m32, a->m33, a->m34,
                         a->m42, a->m43, a->m44);
    minor_m12 = MAT3_DET(a->m21, a->m23, a->m24,
                         a->m31, a->m33, a->m34,
                         a->m41, a->m43, a->m44);
    minor_m13 = MAT3_DET(a->m21, a->m22, a->m24,
                         a->m31, a->m32, a->m34,
                         a->m41, a->m42, a->m44);
    minor_m14 = MAT3_DET(a->m21, a->m22, a->m23,
                         a->m31, a->m32, a->m33,
                         a->m41, a->m42, a->m43);

    result = ((a->m11 * minor_m11) -
              (a->m12 * minor_m12) +
              (a->m13 * minor_m13) -
              (a->m14 * minor_m14));
    
    return result;
}

#undef MAT3_DET

TmMat4 *
tmMat4Identity(TmMat4 *dest)
{
    dest->m11 = 1.0f;
    dest->m21 = 0.0f;
    dest->m31 = 0.0f;
    dest->m41 = 0.0f;

    dest->m12 = 0.0f;
    dest->m22 = 1.0f;
    dest->m32 = 0.0f;
    dest->m42 = 0.0f;

    dest->m13 = 0.0f;
    dest->m23 = 0.0f;
    dest->m33 = 1.0f;
    dest->m43 = 0.0f;

    dest->m14 = 0.0f;
    dest->m24 = 0.0f;
    dest->m34 = 0.0f;
    dest->m44 = 1.0f;

    return dest;
}

TmMat4 *
tmMat4Inverse(TmMat4 *dest, TmMat4 const *a)
{
    TmScalar determinant;

    // TODO: This is not finished
    assert(0);

    determinant = tmMat4Determinant(a);
    assert(determinant != 0.0f);
    //*dest = inverse;

    return dest;
}

TmMat4 *
tmMat4LookAt(TmMat4 *dest, TmVec3 const *eye, TmVec3 const *center, TmVec3 const *up)
{
    TmVec3 forward_component;
    TmVec3 right_component;
    TmVec3 up_component;
    TmVec3 up_normalized;

    // TODO: This is not finished
    assert(0);

    tmVec3Sub(&forward_component, center, eye);
    tmVec3Normalize(&forward_component, &forward_component);
    tmVec3Normalize(&up_normalized, up);
    tmVec3Cross(&right_component, &forward_component, &up_normalized);
    tmVec3Normalize(&right_component, &right_component);
    tmVec3Cross(&up_component, &right_component, &forward_component);
    /*
     * Don't need to normalize up_component, since we know
     * right_component and forward_component are both unit length
     * and are perpendicular.
     *
     * |right x forward| = |right| * |forward| * sin(pi/2) 
     *                   = 1       * 1         * 1
     */
    //tmVec3Normalize(&up_component, &up_component)
    dest->m11 = right_component.x;
    dest->m21 = up_component.x;
    dest->m31 = -forward_component.x;
    dest->m41 = 0.0f;
    dest->m12 = right_component.y;
    dest->m22 = up_component.y;
    dest->m32 = -forward_component.y;
    dest->m42 = 0.0f;
    dest->m13 = right_component.z;
    dest->m23 = up_component.z;
    dest->m33 = -forward_component.z;
    dest->m43 = 0.0f;
    dest->m14 = -tmVec3Dot(&right_component, eye);
    dest->m14 = -tmVec3Dot(&up_component, eye);
    dest->m14 = tmVec3Dot(&forward_component, eye);
    dest->m14 = 1.0f;

    return dest;
}

TmMat4 *
tmMat4Perspective(TmMat4 *dest, TmScalar fovY, TmScalar aspectRatio, TmScalar zNear, TmScalar zFar)
{
    TmScalar fovYRadians;
    TmScalar cotOfHalfFovY;

    // TODO: this is not finished
    assert(0);

    fovYRadians = (180.0f * fovY) / TM_SCALAR_PI;
    cotOfHalfFovY = 1.0f / tmScalarTan(fovYRadians / 2.0f);

    assert(aspectRatio >= 0.0f);
    assert(zNear - zFar != 0.0f);
    dest->m11 = cotOfHalfFovY / aspectRatio;
    dest->m21 = 0.0f;
    dest->m31 = 0.0f;
    dest->m41 = 0.0f;
    dest->m12 = 0.0f;
    dest->m22 = cotOfHalfFovY;
    dest->m32 = 0.0f;
    dest->m42 = 0.0f;
    dest->m13 = 0.0f;
    dest->m23 = 0.0f;
    dest->m33 = (zFar + zNear) / (zNear - zFar);
    dest->m43 = -1.0f;
    dest->m14 = 0.0f;
    dest->m24 = 0.0f;
    dest->m34 = (2.0f * zFar * zNear) / (zNear - zFar);
    dest->m44 = 0.0f;

    return dest;
}
