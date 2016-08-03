#include "scalar.h"

TmScalar
tmScalarMix(TmScalar x, TmScalar y, TmScalar a)
{
    return x * (1.0f - a) + y * a;
}

