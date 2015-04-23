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

	suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
