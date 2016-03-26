#include <assert.h>
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

#define TEST_FLOAT_EPSILON (0.000001f)

#define ASSERT_FLOAT_APPROX_EQ(float1, float2)                            \
    do {                                                                  \
        ck_assert_msg(tmScalarFabs(float1 - float2) < TEST_FLOAT_EPSILON, \
                      "(" #float1 "=%f) !~= (" #float2 "=%f)",            \
                      float1,                                             \
                      float2);                                            \
    } while (0)

START_TEST(matrixSizes)
{
    /**
     * Double-check that there are no issues with padding or
     * packing that would interfere with using the vector
     * structs directly, as opposed to arrays of floats.
     */
    ck_assert_int_eq(sizeof(TmMat2), sizeof(TmScalar[4]));
    ck_assert_int_eq(sizeof(TmMat3), sizeof(TmScalar[9]));
    ck_assert_int_eq(sizeof(TmMat4), sizeof(TmScalar[16]));
}
END_TEST

START_TEST(mat2Multiply01)
{
    TmMat2 const a = {1.0f, 5.0f, -2.0f, 0.0f};
    TmMat2 const b = {-3.0f, 4.0f, 7.0f, 1.0f/3.0f};
    TmMat2 dest;

    tmMat2Multiply(&dest, &a, &b);

    ASSERT_FLOAT_APPROX_EQ(dest.m11, -11.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m21, -15.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m12, 19.0f/3.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m22, 35.0f);
}
END_TEST

START_TEST(mat2Inverse01)
{
    TmMat2 const a = {4.0f, 3.0f, 3.0f, 2.0f};
    TmMat2 inverse;

    tmMat2Inverse(&inverse, &a);

    ASSERT_FLOAT_APPROX_EQ(inverse.m11, -2.0f);
    ASSERT_FLOAT_APPROX_EQ(inverse.m21, 3.0f);
    ASSERT_FLOAT_APPROX_EQ(inverse.m12, 3.0f);
    ASSERT_FLOAT_APPROX_EQ(inverse.m22, -4.0f);
}
END_TEST

START_TEST(mat2Inverse02)
{
    TmMat2 const a = {1.0f, 3.0f, 2.0f, 4.0f};
    TmMat2 inverse;

    tmMat2Inverse(&inverse, &a);

    ASSERT_FLOAT_APPROX_EQ(inverse.m11, -2.0f);
    ASSERT_FLOAT_APPROX_EQ(inverse.m21, 1.5f);
    ASSERT_FLOAT_APPROX_EQ(inverse.m12, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(inverse.m22, -0.5f);
}
END_TEST

START_TEST(mat4Rotation01)
{
    TmMat4 dest;
    TmVec3 const normal = {1.0f, 0.0f, 0.0f};
    TmScalar angle;

    angle = (20.0f * TM_SCALAR_PI) / 180.0f;
    tmMat4Rotation(&dest, &normal, angle);

    ASSERT_FLOAT_APPROX_EQ(dest.m11, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m21, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m31, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m41, 0.0f);

    ASSERT_FLOAT_APPROX_EQ(dest.m12, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m22, 0.939693f);
    ASSERT_FLOAT_APPROX_EQ(dest.m32, 0.342020f);
    ASSERT_FLOAT_APPROX_EQ(dest.m42, 0.0f);

    ASSERT_FLOAT_APPROX_EQ(dest.m13, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m23, -0.342020f);
    ASSERT_FLOAT_APPROX_EQ(dest.m33, 0.939693f);
    ASSERT_FLOAT_APPROX_EQ(dest.m43, 0.0f);

    ASSERT_FLOAT_APPROX_EQ(dest.m14, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m24, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m34, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m44, 1.0f);
}
END_TEST

START_TEST(mat4Identity01)
{
    TmMat4 dest;

    tmMat4Identity(&dest);

    ASSERT_FLOAT_APPROX_EQ(dest.m11, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m21, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m31, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m41, 0.0f);

    ASSERT_FLOAT_APPROX_EQ(dest.m12, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m22, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m32, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m42, 0.0f);

    ASSERT_FLOAT_APPROX_EQ(dest.m13, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m23, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m33, 1.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m43, 0.0f);

    ASSERT_FLOAT_APPROX_EQ(dest.m14, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m24, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m34, 0.0f);
    ASSERT_FLOAT_APPROX_EQ(dest.m44, 1.0f);
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

    suite = suite_create("matrix");

    testCase = tcase_create("matrix sizes");
    tcase_add_test(testCase, matrixSizes);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("mat2 multiply");
    tcase_add_test(testCase, mat2Multiply01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("mat2 inverse");
    tcase_add_test(testCase, mat2Inverse01);
    tcase_add_test(testCase, mat2Inverse02);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("mat4 rotation");
    tcase_add_test(testCase, mat4Rotation01);
    suite_add_tcase(suite, testCase);

    testCase = tcase_create("mat4 identity");
    tcase_add_test(testCase, mat4Identity01);
    suite_add_tcase(suite, testCase);

    suiteRunner = srunner_create(suite);
    srunner_run_all(suiteRunner, CK_NORMAL);
    numberFailed = srunner_ntests_failed(suiteRunner);
    srunner_free(suiteRunner);

    return ((numberFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
