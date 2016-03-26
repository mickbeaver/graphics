#include <assert.h>
#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

#define TEST_FLOAT_EPSILON (0.000001f)

#define ASSERT_FLOAT_APPROX_EQ(float1, float2) do {                  \
    ck_assert_msg(tmScalarFabs(float1 - float2) < TEST_FLOAT_EPSILON, \
                  "(" #float1 "=%f) !~= (" #float2 "=%f)",           \
                  float1,                                            \
                  float2);                                           \
} while (0)

START_TEST(vectorSizes)
{
    /**
     * Double-check that there are no issues with padding or
     * packing that would interfere with using the vector
     * structs directly, as opposed to arrays of floats.
     */
    ck_assert_int_eq(sizeof(TmVec2), sizeof(TmScalar[2]));
    ck_assert_int_eq(sizeof(TmVec3), sizeof(TmScalar[3]));
    ck_assert_int_eq(sizeof(TmVec4), sizeof(TmScalar[4]));
}
END_TEST

START_TEST(vec2Add01)
{
    TmVec2 p = {1.0f, 1.0f};
    TmVec2 q = {1.0f, 1.0f};
    TmVec2 dest;

    tmVec2Add(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 2.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 2.0f);
}
END_TEST

START_TEST(vec2Distance01)
{
    TmVec2 p = {10.0f, 6.0f};
    TmVec2 q = {14.0f, 30.0f};
    TmScalar distance;

    distance = tmVec2Distance(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(distance, 24.331051f);
}
END_TEST

START_TEST(vec2Distance02)
{
    TmVec2 p = {0.0f, 0.0f};
    TmVec2 q = {-12.0f, 5.0f};
    TmScalar distance;

    distance = tmVec2Distance(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(distance, 13.0f);
}
END_TEST

START_TEST(vec2Dot01)
{
    TmVec2 p = {2.0f, 1.25f};
    TmVec2 q = {1.0f, -3.0f};
    TmScalar dotProduct;

    dotProduct = tmVec2Dot(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(dotProduct, -1.75f);
}
END_TEST

START_TEST(vec2Length01)
{
    TmVec2 p = {-12.0f, 5.0f};
    TmScalar length;

    length = tmVec2Length(&p);

    ASSERT_FLOAT_APPROX_EQ(length, 13.0f);
}
END_TEST

START_TEST(vec2Normalize01)
{
    TmVec2 p = {12.0f, 5.0f};

    tmVec2Normalize(&p, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 0.923077f);
    ASSERT_FLOAT_APPROX_EQ(p.y, 0.384615f);
    ASSERT_FLOAT_APPROX_EQ(tmVec2Length(&p), 1.0f);
}
END_TEST

START_TEST(vec2Normalize02)
{
    TmVec2 p = {0.0f, 743.632f};

    tmVec2Normalize(&p, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(p.y, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(tmVec2Length(&p), 1.0f);
}
END_TEST

START_TEST(vec2Projection01)
{
    TmVec2 p = {4.0f, 3.0f};
    TmVec2 q;
    TmVec2 pOntoQ;

    q.x = tmScalarSqrt(2.0f) / 2.0f;
    q.y = tmScalarSqrt(2.0f) / 2.0f;
    tmVec2Projection(&pOntoQ, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(pOntoQ.x, 3.5f);
    ASSERT_FLOAT_APPROX_EQ(pOntoQ.y, 3.5f);
}
END_TEST

START_TEST(vec2Scale01)
{
    TmVec2 p = {4.0f, -7.0f};

    tmVec2Scale(&p, 3.0f, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
    ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
}
END_TEST

START_TEST(vec2Sub01)
{
    TmVec2 p = {3.0f, 10.0f};
    TmVec2 q = {8.0f, -7.0f};
    TmVec2 dest;

    tmVec2Sub(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
}
END_TEST

START_TEST(vec2Sub02)
{
    TmVec2 p = {4.0f, 5.0f};
    TmVec2 q = {-4.0f, -5.0f};
    TmVec2 dest;

    tmVec2Sub(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
}
END_TEST

START_TEST(vec3Add01)
{
    TmVec3 p = {7.0f, -2.0f, -3.0f};
    TmVec3 q = {6.0f, 6.0f, -4.0f};
    TmVec3 dest;

    tmVec3Add(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 13.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 4.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, -7.0f);
}
END_TEST

START_TEST(vec3Add02)
{
    TmVec3 p = {2.0f, 9.0f, -1.0f};
    TmVec3 q = {-2.0f, -9.0f, 1.0f};
    TmVec3 dest;

    tmVec3Add(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, 0.0f);
}
END_TEST

START_TEST(vec3Cross01)
{
    TmVec3 xAxis = {1.0f, 0.0f, 0.0f};
    TmVec3 yAxis = {0.0f, 1.0f, 0.0f};
    TmVec3 zAxis;

    tmVec3Cross(&zAxis, &xAxis, &yAxis);

    ASSERT_FLOAT_APPROX_EQ(zAxis.x, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(zAxis.y, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(zAxis.z, 1.0f);
}
END_TEST

START_TEST(vec3Cross02)
{
    TmVec3 p = {2.0f, 2.0f, 1.0f};
    TmVec3 q = {1.0f, -2.0f, 0.0f};
    TmVec3 dest;

    tmVec3Cross(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 2.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, -6.0f);
}
END_TEST

START_TEST(vec3Distance01)
{
    TmVec3 p = {3.0f, 10.0f, 7.0f};
    TmVec3 q = {8.0f, -7.0f, 4.0f};
    TmScalar distance;

    distance = tmVec3Distance(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(distance, 17.972200f);
}
END_TEST

START_TEST(vec3Distance02)
{
    TmVec3 p = {-2.0f, -4.0f, 9.0f};
    TmVec3 q = {6.0f, -7.0f, 9.5f};
    TmScalar distance;

    distance = tmVec3Distance(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(distance, 8.558621f);
}
END_TEST

START_TEST(vec3Dotproduct01)
{
    TmVec3 p = {2.0f, 2.0f, 1.0f};
    TmVec3 q = {1.0f, -2.0f, 0.0f};
    TmScalar result;

    result = tmVec3Dot(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(result, -2.0f);
}
END_TEST

START_TEST(vec3Length01)
{
    TmVec3 p = {8.0f, -3.0f, 0.5f};
    TmScalar length;

    length = tmVec3Length(&p);

    ASSERT_FLOAT_APPROX_EQ(length, 8.558621384311845f);
}
END_TEST

START_TEST(vec3Normalize01)
{
    TmVec3 p = {8.0f, -3.0f, 0.5f};

    tmVec3Normalize(&p, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 0.934730f);
    ASSERT_FLOAT_APPROX_EQ(p.y, -0.350524f);
    ASSERT_FLOAT_APPROX_EQ(p.z, 0.058421f);
    ASSERT_FLOAT_APPROX_EQ(tmVec3Length(&p), 1.0f);
}
END_TEST

START_TEST(vec3Normalize02)
{
    TmVec3 p = {-12.0f, 3.0f, -4.0f};

    tmVec3Normalize(&p, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, -0.923077f);
    ASSERT_FLOAT_APPROX_EQ(p.y, 0.23077f);
    ASSERT_FLOAT_APPROX_EQ(p.z, -0.307692f);
    ASSERT_FLOAT_APPROX_EQ(tmVec3Length(&p), 1.0f);
}
END_TEST

START_TEST(vec3Projection01)
{
    TmVec3 p = {4.0f, 3.0f, -1.0f};
    TmVec3 q;
    TmVec3 pOntoQ;

    q.x = tmScalarSqrt(2.0f) / 2.0f;
    q.y = tmScalarSqrt(2.0f) / 2.0f;
    q.z = 0.0f;
    tmVec3Projection(&pOntoQ, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(pOntoQ.x, 3.5f);
    ASSERT_FLOAT_APPROX_EQ(pOntoQ.y, 3.5f);
    ASSERT_FLOAT_APPROX_EQ(pOntoQ.z, 0.0f);
}
END_TEST

START_TEST(vec3Scale01)
{
    TmVec3 p = {4.0f, -7.0f, 0.0f};

    tmVec3Scale(&p, 3.0f, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
    ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
    ASSERT_FLOAT_APPROX_EQ(p.z, 0.0f);
}
END_TEST

START_TEST(vec3Sub01)
{
    TmVec3 p = {3.0f, 10.0f, 7.0f};
    TmVec3 q = {8.0f, -7.0f, 4.0f};
    TmVec3 dest;

    tmVec3Sub(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, 3.0f);
}
END_TEST

START_TEST(vec3Sub02)
{
    TmVec3 p = {4.0f, 5.0f, -11.0f};
    TmVec3 q = {-4.0f, -5.0f, 11.0f};
    TmVec3 dest;

    tmVec3Sub(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, -22.0f);
}
END_TEST

START_TEST(vec4Add01)
{
    TmVec4 p = {7.0f, -2.0f, -3.0f, -8.2f};
    TmVec4 q = {6.0f, 6.0f, -4.0f, -7.11f};
    TmVec4 dest;

    tmVec4Add(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 13.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 4.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, -7.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.w, -15.309999f);
}
END_TEST

START_TEST(vec4Add02)
{
    TmVec4 p = {2.0f, 9.0f, -1.0f, -2.5f};
    TmVec4 q = {-2.0f, -9.0f, 1.0f, 23.0f};
    TmVec4 dest;

    tmVec4Add(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.w, 20.5f);
}
END_TEST

START_TEST(vec4Distance01)
{
    TmVec4 p = {4.0f, -4.0f, -4.0f, 4.0f};
    TmVec4 q = {-6.0f, 6.0f, 6.0f, -6.0f};
    TmScalar distance;

    distance = tmVec4Distance(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(distance, 20.0f);
}
END_TEST

START_TEST(vec4DotProduct01)
{
    TmVec4 p = {2.0f, 2.0f, 1.0f, 3.5f};
    TmVec4 q = {1.0f, -2.0f, 0.0f, 12.0f};
    TmScalar result;

    result = tmVec4Dot(&p, &q);

    ASSERT_FLOAT_APPROX_EQ(result, 40.0f);
}
END_TEST

START_TEST(vec4Length01)
{
    TmVec4 p = {8.0f, -3.0f, 0.5f, 2.5f};
    TmScalar length;

    length = tmVec4Length(&p);

    ASSERT_FLOAT_APPROX_EQ(length, 8.91627725f);
}
END_TEST

START_TEST(vec4Normalize01)
{
    TmVec4 p = {1.0f, 1.0f, 1.0f, 1.0f};

    tmVec4Normalize(&p, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 0.5f);
    ASSERT_FLOAT_APPROX_EQ(p.y, 0.5f);
    ASSERT_FLOAT_APPROX_EQ(p.z, 0.5f);
    ASSERT_FLOAT_APPROX_EQ(p.w, 0.5f);
    ASSERT_FLOAT_APPROX_EQ(tmVec4Length(&p), 1.0f);
}
END_TEST

START_TEST(vec4Scale01)
{
    TmVec4 p = {4.0f, -7.0f, 1.0f, -2.5};

    tmVec4Scale(&p, 3.0f, &p);

    ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
    ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
    ASSERT_FLOAT_APPROX_EQ(p.z, 3.0f);
    ASSERT_FLOAT_APPROX_EQ(p.w, -7.5f);
}
END_TEST

START_TEST(vec4Sub01)
{
    TmVec4 p = {3.0f, 10.0f, 7.0f, 23.0f};
    TmVec4 q = {8.0f, -7.0f, 4.0f, 12.0f};
    TmVec4 dest;

    tmVec4Sub(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, 3.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.w, 11.0f);
}
END_TEST

START_TEST(vec4Sub02)
{
    TmVec4 p = {4.0f, 5.0f, -11.0f, -17.0f};
    TmVec4 q = {-4.0f, -5.0f, 11.0f, 9.5f};
    TmVec4 dest;

    tmVec4Sub(&dest, &p, &q);

    ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.z, -22.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.w, -26.5f);
}
END_TEST

int
main(int argc, char *argv[])
{
    int numberFailed;
    Suite *suite;
    SRunner *suiteRunner;
    TCase *testCase;

    (void) argc;
    (void) argv;

    suite = suite_create("vector");

    testCase = tcase_create("vector sizes");
    tcase_add_test(testCase, vectorSizes);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 addition");
    tcase_add_test(testCase, vec2Add01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 distance");
    tcase_add_test(testCase, vec2Distance01);
    tcase_add_test(testCase, vec2Distance02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 dot product");
    tcase_add_test(testCase, vec2Dot01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 length");
    tcase_add_test(testCase, vec2Length01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 normalize");
    tcase_add_test(testCase, vec2Normalize01);
    tcase_add_test(testCase, vec2Normalize02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 projection");
    tcase_add_test(testCase, vec2Projection01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 scale");
    tcase_add_test(testCase, vec2Scale01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec2 subtraction");
    tcase_add_test(testCase, vec2Sub01);
    tcase_add_test(testCase, vec2Sub02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 addition");
    tcase_add_test(testCase, vec3Add01);
    tcase_add_test(testCase, vec3Add02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 cross product");
    tcase_add_test(testCase, vec3Cross01);
    tcase_add_test(testCase, vec3Cross02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 distance");
    tcase_add_test(testCase, vec3Distance01);
    tcase_add_test(testCase, vec3Distance02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 dot product");
    tcase_add_test(testCase, vec3Dotproduct01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 length");
    tcase_add_test(testCase, vec3Length01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 normalize");
    tcase_add_test(testCase, vec3Normalize01);
    tcase_add_test(testCase, vec3Normalize02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 projection");
    tcase_add_test(testCase, vec3Projection01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 scale");
    tcase_add_test(testCase, vec3Scale01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec3 subtraction");
    tcase_add_test(testCase, vec3Sub01);
    tcase_add_test(testCase, vec3Sub02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 addition");
    tcase_add_test(testCase, vec4Add01);
    tcase_add_test(testCase, vec4Add02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 distance");
    tcase_add_test(testCase, vec4Distance01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 dot product");
    tcase_add_test(testCase, vec4DotProduct01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 length");
    tcase_add_test(testCase, vec4Length01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 scale");
    tcase_add_test(testCase, vec4Scale01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 normalize");
    tcase_add_test(testCase, vec4Normalize01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("vec4 subtraction");
    tcase_add_test(testCase, vec4Sub01);
    tcase_add_test(testCase, vec4Sub02);
    suite_add_tcase(suite, testCase);

    suiteRunner = srunner_create(suite);
    srunner_run_all(suiteRunner, CK_NORMAL);
    numberFailed = srunner_ntests_failed(suiteRunner);
    srunner_free(suiteRunner);

    return ((numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
