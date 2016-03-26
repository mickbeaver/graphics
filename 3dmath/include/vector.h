#ifndef GRAPHICS_MATH_VECTOR_H
#define GRAPHICS_MATH_VECTOR_H

#include "scalar.h"

typedef struct TmVec2 {
    TmScalar x;
    TmScalar y;
} TmVec2;

typedef struct TmVec3 {
    TmScalar x;
    TmScalar y;
    TmScalar z;
} TmVec3;

typedef struct TmVec4 {
    TmScalar x;
    TmScalar y;
    TmScalar z;
    TmScalar w;
} TmVec4;

TmVec2      *tmVec2Add(TmVec2 *dest, TmVec2 const *p, TmVec2 const *q);
TmScalar     tmVec2Distance(TmVec2 const *p, TmVec2 const *q);
TmScalar     tmVec2Dot(TmVec2 const *p, TmVec2 const *q);
TmScalar     tmVec2Length(TmVec2 const *p);
TmVec2      *tmVec2Normalize(TmVec2 *dest, TmVec2 const *p);
TmVec2      *tmVec2Projection(TmVec2 *pOntoQ,
                              TmVec2 const *p,
                              TmVec2 const *q);
TmVec2      *tmVec2Scale(TmVec2 *dest, TmScalar scale, TmVec2 const *q);
TmScalar     tmVec2SqLength(TmVec2 const *p);
TmVec2      *tmVec2Sub(TmVec2 *dest, TmVec2 const *p, TmVec2 const *q);

TmVec3      *tmVec3Add(TmVec3 *dest, TmVec3 const *p, TmVec3 const *q);
TmScalar     tmVec3Distance(TmVec3 const *p, TmVec3 const *q);
TmVec3      *tmVec3Cross(TmVec3 *dest, TmVec3 const *p, TmVec3 const *q);
TmScalar     tmVec3Dot(TmVec3 const *p, TmVec3 const *q);
TmScalar     tmVec3Length(TmVec3 const *p);
TmVec3      *tmVec3Normalize(TmVec3 *dest, TmVec3 const *p);
TmVec3      *tmVec3Projection(TmVec3 *pOntoQ, TmVec3 const *p, TmVec3 const *q);
TmVec3      *tmVec3Scale(TmVec3 *dest, TmScalar scale, TmVec3 const *q);
TmScalar     tmVec3SqLength(TmVec3 const *p);
TmVec3      *tmVec3Sub(TmVec3 *dest, TmVec3 const *p, TmVec3 const *q);

TmVec4      *tmVec4Add(TmVec4 *dest, TmVec4 const *p, TmVec4 const *q);
TmScalar     tmVec4Distance(TmVec4 const *p, TmVec4 const *q);
TmScalar     tmVec4Dot(TmVec4 const *p, TmVec4 const *q);
TmScalar     tmVec4Length(TmVec4 const *p);
TmVec4      *tmVec4Normalize(TmVec4 *dest, TmVec4 const *p);
TmVec4      *tmVec4Projection(TmVec4 *pOntoQ, TmVec4 const *p, TmVec4 const *q);
TmVec4      *tmVec4Scale(TmVec4 *dest, TmScalar scale, TmVec4 const *q);
TmScalar     tmVec4SqLength(TmVec4 const *p);
TmVec4      *tmVec4Sub(TmVec4 *dest, TmVec4 const *p, TmVec4 const *q);

#endif /* GRAPHICS_MATH_VECTOR_H */
