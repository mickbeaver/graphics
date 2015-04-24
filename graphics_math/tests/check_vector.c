#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "vector.h"

START_TEST(test_vec2_add)
{
	VEC2 addend_one, addend_two, sum;
	addend_one = (VEC2){.x=1.0f, .y=1.0f};
	addend_two = (VEC2){.x=1.0f, .y=1.0f};
	vec2_add(&sum, &addend_one, &addend_two);
	ck_assert(sum.x == 2.0f);
	ck_assert(sum.y == 2.0f);
}
END_TEST

START_TEST(test_vec3_cross_1)
{
	VEC3 x_axis, y_axis, z_axis;
	x_axis = (VEC3){.x=1.0f, .y=0.0f, .z=0.0f};
	y_axis = (VEC3){.x=0.0f, .y=1.0f, .z=0.0f};
	vec3_cross(&z_axis, &x_axis, &y_axis);
	ck_assert_msg(z_axis.x == 1.0f, "z_axis.x=%f", z_axis.x);
	ck_assert(z_axis.y == 0.0f);
	ck_assert(z_axis.z == 1.0f);
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

	suite = suite_create("vector");

	test_case = tcase_create("addition");
	tcase_add_test(test_case, test_vec2_add);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3_cross_1");
	tcase_add_test(test_case, test_vec3_cross_1);
	suite_add_tcase(suite, test_case);
	
	suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
