#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"
#include "vector.h"

#define TEST_FLOAT_EPSILON (0.000001f)

TEST
vectorSizes(void)
{
    /**
     * Double-check that there are no issues with padding or
     * packing that would interfere with using the vector
     * structs directly, as opposed to arrays of floats.
     */
    ASSERT_EQ(sizeof(TmVec2), sizeof(TmScalar[2]));
    ASSERT_EQ(sizeof(TmVec3), sizeof(TmScalar[3]));
    ASSERT_EQ(sizeof(TmVec4), sizeof(TmScalar[4]));
    PASS();
}

TEST
vec2Add01(void)
{
    TmVec2 p = {1.0f, 1.0f};
    TmVec2 q = {1.0f, 1.0f};
    TmVec2 dest;

    tmVec2Add(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 2.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 2.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Distance01(void)
{
    TmVec2 p = {10.0f, 6.0f};
    TmVec2 q = {14.0f, 30.0f};
    TmScalar distance;

    distance = tmVec2Distance(&p, &q);

    ASSERT_IN_RANGE(distance, 24.331051f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Distance02(void)
{
    TmVec2 p = {0.0f, 0.0f};
    TmVec2 q = {-12.0f, 5.0f};
    TmScalar distance;

    distance = tmVec2Distance(&p, &q);

    ASSERT_IN_RANGE(distance, 13.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Dot01(void)
{
    TmVec2 p = {2.0f, 1.25f};
    TmVec2 q = {1.0f, -3.0f};
    TmScalar dotProduct;

    dotProduct = tmVec2Dot(&p, &q);

    ASSERT_IN_RANGE(dotProduct, -1.75f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Length01(void)
{
    TmVec2 p = {-12.0f, 5.0f};
    TmScalar length;

    length = tmVec2Length(&p);

    ASSERT_IN_RANGE(length, 13.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Normalize01(void)
{
    TmVec2 p = {12.0f, 5.0f};

    tmVec2Normalize(&p, &p);

    ASSERT_IN_RANGE(p.x, 0.923077f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, 0.384615f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(tmVec2Length(&p), 1.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Normalize02(void)
{
    TmVec2 p = {0.0f, 743.632f};

    tmVec2Normalize(&p, &p);

    ASSERT_IN_RANGE(p.x, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(tmVec2Length(&p), 1.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Projection01(void)
{
    TmVec2 p = {4.0f, 3.0f};
    TmVec2 q;
    TmVec2 pOntoQ;

    q.x = tmScalarSqrt(2.0f) / 2.0f;
    q.y = tmScalarSqrt(2.0f) / 2.0f;
    tmVec2Projection(&pOntoQ, &p, &q);

    ASSERT_IN_RANGE(pOntoQ.x, 3.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(pOntoQ.y, 3.5f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Scale01(void)
{
    TmVec2 p = {4.0f, -7.0f};

    tmVec2Scale(&p, 3.0f, &p);

    ASSERT_IN_RANGE(p.x, 12.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, -21.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Sub01(void)
{
    TmVec2 p = {3.0f, 10.0f};
    TmVec2 q = {8.0f, -7.0f};
    TmVec2 dest;

    tmVec2Sub(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, -5.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 17.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec2Sub02(void)
{
    TmVec2 p = {4.0f, 5.0f};
    TmVec2 q = {-4.0f, -5.0f};
    TmVec2 dest;

    tmVec2Sub(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 8.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 10.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Add01(void)
{
    TmVec3 p = {7.0f, -2.0f, -3.0f};
    TmVec3 q = {6.0f, 6.0f, -4.0f};
    TmVec3 dest;

    tmVec3Add(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 13.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 4.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, -7.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Add02(void)
{
    TmVec3 p = {2.0f, 9.0f, -1.0f};
    TmVec3 q = {-2.0f, -9.0f, 1.0f};
    TmVec3 dest;

    tmVec3Add(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, 0.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Cross01(void)
{
    TmVec3 xAxis = {1.0f, 0.0f, 0.0f};
    TmVec3 yAxis = {0.0f, 1.0f, 0.0f};
    TmVec3 zAxis;

    tmVec3Cross(&zAxis, &xAxis, &yAxis);

    ASSERT_IN_RANGE(zAxis.x, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(zAxis.y, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(zAxis.z, 1.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Cross02(void)
{
    TmVec3 p = {2.0f, 2.0f, 1.0f};
    TmVec3 q = {1.0f, -2.0f, 0.0f};
    TmVec3 dest;

    tmVec3Cross(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 2.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, -6.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Distance01(void)
{
    TmVec3 p = {3.0f, 10.0f, 7.0f};
    TmVec3 q = {8.0f, -7.0f, 4.0f};
    TmScalar distance;

    distance = tmVec3Distance(&p, &q);

    ASSERT_IN_RANGE(distance, 17.972200f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Distance02(void)
{
    TmVec3 p = {-2.0f, -4.0f, 9.0f};
    TmVec3 q = {6.0f, -7.0f, 9.5f};
    TmScalar distance;

    distance = tmVec3Distance(&p, &q);

    ASSERT_IN_RANGE(distance, 8.558621f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3DotProduct01(void)
{
    TmVec3 p = {2.0f, 2.0f, 1.0f};
    TmVec3 q = {1.0f, -2.0f, 0.0f};
    TmScalar result;

    result = tmVec3Dot(&p, &q);

    ASSERT_IN_RANGE(result, -2.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Length01(void)
{
    TmVec3 p = {8.0f, -3.0f, 0.5f};
    TmScalar length;

    length = tmVec3Length(&p);

    ASSERT_IN_RANGE(length, 8.558621384311845f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Normalize01(void)
{
    TmVec3 p = {8.0f, -3.0f, 0.5f};

    tmVec3Normalize(&p, &p);

    ASSERT_IN_RANGE(p.x, 0.934730f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, -0.350524f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.z, 0.058421f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(tmVec3Length(&p), 1.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Normalize02(void)
{
    TmVec3 p = {-12.0f, 3.0f, -4.0f};

    tmVec3Normalize(&p, &p);

    ASSERT_IN_RANGE(p.x, -0.923077f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, 0.23077f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.z, -0.307692f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(tmVec3Length(&p), 1.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Projection01(void)
{
    TmVec3 p = {4.0f, 3.0f, -1.0f};
    TmVec3 q;
    TmVec3 pOntoQ;

    q.x = tmScalarSqrt(2.0f) / 2.0f;
    q.y = tmScalarSqrt(2.0f) / 2.0f;
    q.z = 0.0f;
    tmVec3Projection(&pOntoQ, &p, &q);

    ASSERT_IN_RANGE(pOntoQ.x, 3.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(pOntoQ.y, 3.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(pOntoQ.z, 0.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Scale01(void)
{
    TmVec3 p = {4.0f, -7.0f, 0.0f};

    tmVec3Scale(&p, 3.0f, &p);

    ASSERT_IN_RANGE(p.x, 12.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, -21.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.z, 0.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Sub01(void)
{
    TmVec3 p = {3.0f, 10.0f, 7.0f};
    TmVec3 q = {8.0f, -7.0f, 4.0f};
    TmVec3 dest;

    tmVec3Sub(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, -5.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 17.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, 3.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec3Sub02(void)
{
    TmVec3 p = {4.0f, 5.0f, -11.0f};
    TmVec3 q = {-4.0f, -5.0f, 11.0f};
    TmVec3 dest;

    tmVec3Sub(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 8.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 10.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, -22.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Add01(void)
{
    TmVec4 p = {7.0f, -2.0f, -3.0f, -8.2f};
    TmVec4 q = {6.0f, 6.0f, -4.0f, -7.11f};
    TmVec4 dest;

    tmVec4Add(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 13.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 4.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, -7.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.w, -15.309999f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Add02(void)
{
    TmVec4 p = {2.0f, 9.0f, -1.0f, -2.5f};
    TmVec4 q = {-2.0f, -9.0f, 1.0f, 23.0f};
    TmVec4 dest;

    tmVec4Add(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.w, 20.5f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Distance01(void)
{
    TmVec4 p = {4.0f, -4.0f, -4.0f, 4.0f};
    TmVec4 q = {-6.0f, 6.0f, 6.0f, -6.0f};
    TmScalar distance;

    distance = tmVec4Distance(&p, &q);

    ASSERT_IN_RANGE(distance, 20.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4DotProduct01(void)
{
    TmVec4 p = {2.0f, 2.0f, 1.0f, 3.5f};
    TmVec4 q = {1.0f, -2.0f, 0.0f, 12.0f};
    TmScalar result;

    result = tmVec4Dot(&p, &q);

    ASSERT_IN_RANGE(result, 40.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Length01(void)
{
    TmVec4 p = {8.0f, -3.0f, 0.5f, 2.5f};
    TmScalar length;

    length = tmVec4Length(&p);

    ASSERT_IN_RANGE(length, 8.91627725f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Normalize01(void)
{
    TmVec4 p = {1.0f, 1.0f, 1.0f, 1.0f};

    tmVec4Normalize(&p, &p);

    ASSERT_IN_RANGE(p.x, 0.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, 0.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.z, 0.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.w, 0.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(tmVec4Length(&p), 1.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Scale01(void)
{
    TmVec4 p = {4.0f, -7.0f, 1.0f, -2.5};

    tmVec4Scale(&p, 3.0f, &p);

    ASSERT_IN_RANGE(p.x, 12.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.y, -21.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.z, 3.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(p.w, -7.5f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Sub01(void)
{
    TmVec4 p = {3.0f, 10.0f, 7.0f, 23.0f};
    TmVec4 q = {8.0f, -7.0f, 4.0f, 12.0f};
    TmVec4 dest;

    tmVec4Sub(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, -5.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 17.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, 3.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.w, 11.0f, TEST_FLOAT_EPSILON);
    PASS();
}

TEST
vec4Sub02(void)
{
    TmVec4 p = {4.0f, 5.0f, -11.0f, -17.0f};
    TmVec4 q = {-4.0f, -5.0f, 11.0f, 9.5f};
    TmVec4 dest;

    tmVec4Sub(&dest, &p, &q);

    ASSERT_IN_RANGE(dest.x, 8.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.y, 10.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.z, -22.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.w, -26.5f, TEST_FLOAT_EPSILON);
    PASS();
}

GREATEST_MAIN_DEFS();

int
main(int argc, char *argv[])
{
    GREATEST_MAIN_BEGIN();

    RUN_TEST(vectorSizes);
    RUN_TEST(vec2Add01);
    RUN_TEST(vec2Distance01);
    RUN_TEST(vec2Distance02);
    RUN_TEST(vec2Dot01);
    RUN_TEST(vec2Length01);
    RUN_TEST(vec2Normalize01);
    RUN_TEST(vec2Normalize02);
    RUN_TEST(vec2Projection01);
    RUN_TEST(vec2Scale01);
    RUN_TEST(vec2Sub01);
    RUN_TEST(vec2Sub02);
    RUN_TEST(vec3Add01);
    RUN_TEST(vec3Add02);
    RUN_TEST(vec3Cross01);
    RUN_TEST(vec3Cross02);
    RUN_TEST(vec3Distance01);
    RUN_TEST(vec3Distance02);
    RUN_TEST(vec3DotProduct01);
    RUN_TEST(vec3Length01);
    RUN_TEST(vec3Normalize01);
    RUN_TEST(vec3Normalize02);
    RUN_TEST(vec3Projection01);
    RUN_TEST(vec3Scale01);
    RUN_TEST(vec3Sub01);
    RUN_TEST(vec3Sub02);
    RUN_TEST(vec4Add01);
    RUN_TEST(vec4Add02);
    RUN_TEST(vec4Distance01);
    RUN_TEST(vec4DotProduct01);
    RUN_TEST(vec4Length01);
    RUN_TEST(vec4Normalize01);
    RUN_TEST(vec4Scale01);
    RUN_TEST(vec4Sub01);
    RUN_TEST(vec4Sub02);

    GREATEST_MAIN_END();
}

