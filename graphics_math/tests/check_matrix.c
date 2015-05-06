#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "matrix.h"

#define TEST_FLOAT_EPSILON (0.0000001f)

#define ASSERT_FLOAT_EQ(float1, float2) do {        \
	ck_assert_msg(float1 == float2,             \
	    "(" #float1 "=%f) != (" #float2 "=%f)", \
	    float1, float2);                        \
} while (0)

#define ASSERT_FLOAT_APPROX_EQ(float1, float2, epsilon) do { \
	ck_assert_msg(abs(float1 - float2) < epsilon,        \
	    "(" #float1 "=%f) !~= (" #float2 "=%f)",         \
	    float1, float2);                                 \
} while (0)

START_TEST(test_matrix_sizes)
{
	/**
	 * Double-check that there are no issues with padding or
	 * packing that would interfere with using the vector
	 * structs directly, as opposed to arrays of floats.
	 */
	ck_assert_int_eq(sizeof(MAT2), sizeof(scalar) * 4);
}
END_TEST

START_TEST(test_mat2_multiply_01)
{
	MAT2 a, b, dest;

	a = (MAT2){.m11=1.0f, .m21=5.0f, .m12=-2.0f, .m22=0.0f};
	b = (MAT2){.m11=-3.0f, .m21=4.0f, .m12=7.0f, .m22=1.0f/3.0f};
	mat2_multiply(&dest, &a, &b);

	ASSERT_FLOAT_EQ(dest.m11, -11.0f);
	ASSERT_FLOAT_EQ(dest.m21, -15.0f);
	ASSERT_FLOAT_EQ(dest.m12, 19.0f/3.0f);
	ASSERT_FLOAT_EQ(dest.m22, 35.0f);
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

	suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
