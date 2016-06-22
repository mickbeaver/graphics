#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "greatest.h"
#include "matrix.h"

#define TEST_FLOAT_EPSILON (0.000001f)

TEST
matrixSizes(void)
{
    /**
     * Double-check that there are no issues with padding or
     * packing that would interfere with using the vector
     * structs directly, as opposed to arrays of floats.
     */
    ASSERT_EQ(sizeof(TmMat2), sizeof(TmScalar[4]));
    ASSERT_EQ(sizeof(TmMat3), sizeof(TmScalar[9]));
    ASSERT_EQ(sizeof(TmMat4), sizeof(TmScalar[16]));
	PASS();
}

TEST
mat2Multiply01(void)
{
    TmMat2 const a = {1.0f, 5.0f, -2.0f, 0.0f};
    TmMat2 const b = {-3.0f, 4.0f, 7.0f, 1.0f/3.0f};
    TmMat2 dest;

    tmMat2Multiply(&dest, &a, &b);

    ASSERT_IN_RANGE(dest.m11, -11.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m21, -15.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m12, 19.0f/3.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m22, 35.0f, TEST_FLOAT_EPSILON);
	PASS();
}

TEST
mat2Inverse01(void)
{
    TmMat2 const a = {4.0f, 3.0f, 3.0f, 2.0f};
    TmMat2 inverse;

    tmMat2Inverse(&inverse, &a);

    ASSERT_IN_RANGE(inverse.m11, -2.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(inverse.m21, 3.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(inverse.m12, 3.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(inverse.m22, -4.0f, TEST_FLOAT_EPSILON);
	PASS();
}

TEST
mat2Inverse02(void)
{
    TmMat2 const a = {1.0f, 3.0f, 2.0f, 4.0f};
    TmMat2 inverse;

    tmMat2Inverse(&inverse, &a);

    ASSERT_IN_RANGE(inverse.m11, -2.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(inverse.m21, 1.5f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(inverse.m12, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(inverse.m22, -0.5f, TEST_FLOAT_EPSILON);
	PASS();
}

TEST
mat4Rotation01(void)
{
    TmMat4 dest;
    TmVec3 const normal = {1.0f, 0.0f, 0.0f};
    TmScalar angle;

    angle = (20.0f * TM_SCALAR_PI) / 180.0f;
    tmMat4Rotation(&dest, &normal, angle);

    ASSERT_IN_RANGE(dest.m11, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m21, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m31, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m41, 0.0f, TEST_FLOAT_EPSILON);

    ASSERT_IN_RANGE(dest.m12, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m22, 0.939693f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m32, 0.342020f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m42, 0.0f, TEST_FLOAT_EPSILON);

    ASSERT_IN_RANGE(dest.m13, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m23, -0.342020f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m33, 0.939693f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m43, 0.0f, TEST_FLOAT_EPSILON);

    ASSERT_IN_RANGE(dest.m14, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m24, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m34, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m44, 1.0f, TEST_FLOAT_EPSILON);
	PASS();
}

TEST
mat4Identity01(void)
{
    TmMat4 dest;

    tmMat4Identity(&dest);

    ASSERT_IN_RANGE(dest.m11, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m21, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m31, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m41, 0.0f, TEST_FLOAT_EPSILON);

    ASSERT_IN_RANGE(dest.m12, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m22, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m32, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m42, 0.0f, TEST_FLOAT_EPSILON);

    ASSERT_IN_RANGE(dest.m13, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m23, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m33, 1.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m43, 0.0f, TEST_FLOAT_EPSILON);

    ASSERT_IN_RANGE(dest.m14, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m24, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m34, 0.0f, TEST_FLOAT_EPSILON);
    ASSERT_IN_RANGE(dest.m44, 1.0f, TEST_FLOAT_EPSILON);
	PASS();
}

GREATEST_MAIN_DEFS();

int
main(int argc, char *argv[])
{
    GREATEST_MAIN_BEGIN();

    RUN_TEST(matrixSizes);
    RUN_TEST(mat2Multiply01);
    RUN_TEST(mat2Inverse01);
    RUN_TEST(mat2Inverse02);
    RUN_TEST(mat4Rotation01);
    RUN_TEST(mat4Identity01);

    GREATEST_MAIN_END();
}
