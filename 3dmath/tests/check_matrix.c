#include <assert.h>
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

#define TEST_FLOAT_EPSILON (0.000001f)

#define ASSERT_FLOAT_APPROX_EQ(float1, float2) do {                      \
	ck_assert_msg(scalar_fabs(float1 - float2) < TEST_FLOAT_EPSILON, \
	"(" #float1 "=%f) !~= (" #float2 "=%f)",			\
	    float1, float2);                                             \
} while (0)

START_TEST(test_matrix_sizes)
{
	/**
	 * Double-check that there are no issues with padding or
	 * packing that would interfere with using the vector
	 * structs directly, as opposed to arrays of floats.
	 */
	ck_assert_int_eq(sizeof(struct mat2), sizeof(scalar[4]));
	ck_assert_int_eq(sizeof(struct mat3), sizeof(scalar[9]));
	ck_assert_int_eq(sizeof(struct mat4), sizeof(scalar[16]));
}
END_TEST

START_TEST(test_mat2_multiply_01)
{
	struct mat2 a = {1.0f, 5.0f, -2.0f, 0.0f};
	struct mat2 b = {-3.0f, 4.0f, 7.0f, 1.0f/3.0f};
	struct mat2 dest;

	mat2_multiply(&dest, &a, &b);

	ASSERT_FLOAT_APPROX_EQ(dest.m11, -11.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.m21, -15.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.m12, 19.0f/3.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.m22, 35.0f);
}
END_TEST

START_TEST(test_mat2_inverse_01)
{
	struct mat2 a = {4.0f, 3.0f, 3.0f, 2.0f};
	struct mat2 inverse;

	mat2_inverse(&inverse, &a);

	ASSERT_FLOAT_APPROX_EQ(inverse.m11, -2.0f);
	ASSERT_FLOAT_APPROX_EQ(inverse.m21, 3.0f);
	ASSERT_FLOAT_APPROX_EQ(inverse.m12, 3.0f);
	ASSERT_FLOAT_APPROX_EQ(inverse.m22, -4.0f);
}
END_TEST

START_TEST(test_mat2_inverse_02)
{
	struct mat2 a = {1.0f, 3.0f, 2.0f, 4.0f};
	struct mat2 inverse;

	mat2_inverse(&inverse, &a);

	ASSERT_FLOAT_APPROX_EQ(inverse.m11, -2.0f);
	ASSERT_FLOAT_APPROX_EQ(inverse.m21, 1.5f);
	ASSERT_FLOAT_APPROX_EQ(inverse.m12, 1.0f);
	ASSERT_FLOAT_APPROX_EQ(inverse.m22, -0.5f);
}
END_TEST

START_TEST(test_mat4_rotation_01)
{
	struct mat4 dest;
	VEC3 normal = {1.0f, 0.0f, 0.0f};
	scalar angle;

	angle = (20.0f * SCALAR_PI) / 180.0f;
	mat4_rotation(&dest, &normal, angle);

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

START_TEST(test_mat4_identity_01)
{
	struct mat4 dest;

	mat4_identity(&dest);

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
	int number_failed;
	Suite *suite;
	SRunner *suite_runner;
	TCase *test_case;

	(void) argc;
	(void) argv;

	suite = suite_create("matrix");

	test_case = tcase_create("matrix sizes");
	tcase_add_test(test_case, test_matrix_sizes);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("mat2 multiply");
	tcase_add_test(test_case, test_mat2_multiply_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("mat2 inverse");
	tcase_add_test(test_case, test_mat2_inverse_01);
	tcase_add_test(test_case, test_mat2_inverse_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("mat4 rotation");
	tcase_add_test(test_case, test_mat4_rotation_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("mat4 identity");
	tcase_add_test(test_case, test_mat4_identity_01);
	suite_add_tcase(suite, test_case);

	suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
