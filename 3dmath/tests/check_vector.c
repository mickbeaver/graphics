#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "libcheck_wrapper.h"
#include "vector.h"

#define TEST_FLOAT_EPSILON (0.000001f)

#define ASSERT_FLOAT_APPROX_EQ(float1, float2) do {                      \
	ck_assert_msg(scalar_fabs(float1 - float2) < TEST_FLOAT_EPSILON, \
	    "(" #float1 "=%f) !~= (" #float2 "=%f)",                     \
	    float1, float2);                                             \
} while (0)

START_TEST(test_vector_sizes)
{
	/**
	 * Double-check that there are no issues with padding or
	 * packing that would interfere with using the vector
	 * structs directly, as opposed to arrays of floats.
	 */
	ck_assert_int_eq(sizeof(VEC2), sizeof(scalar) * 2);
	ck_assert_int_eq(sizeof(VEC3), sizeof(scalar) * 3);
	ck_assert_int_eq(sizeof(VEC4), sizeof(scalar) * 4);
}
END_TEST

START_TEST(test_vec2_add_01)
{
	VEC2 p = {1.0f, 1.0f};
	VEC2 q = {1.0f, 1.0f};
	VEC2 dest;

	vec2_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 2.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 2.0f);
}
END_TEST

START_TEST(test_vec2_distance_01)
{
	VEC2 p = {10.0f, 6.0f};
	VEC2 q = {14.0f, 30.0f};
	scalar distance;

	distance = vec2_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 24.331051f);
}
END_TEST

START_TEST(test_vec2_distance_02)
{
	VEC2 p = {0.0f, 0.0f};
	VEC2 q = {-12.0f, 5.0f};
	scalar distance;

	distance = vec2_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 13.0f);
}
END_TEST

START_TEST(test_vec2_dot_01)
{
	VEC2 p = {2.0f, 1.25f};
	VEC2 q = {1.0f, -3.0f};
	scalar dot_product;

	dot_product = vec2_dot(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(dot_product, -1.75f);
}
END_TEST

START_TEST(test_vec2_length_01)
{
	VEC2 p = {-12.0f, 5.0f};
	scalar length;

	length = vec2_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 13.0f);
}
END_TEST

START_TEST(test_vec2_normalize_01)
{
	VEC2 p = {12.0f, 5.0f};

	vec2_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.923077f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.384615f);
	ASSERT_FLOAT_APPROX_EQ(vec2_length(p), 1.0f);
}
END_TEST

START_TEST(test_vec2_normalize_02)
{
	VEC2 p = {0.0f, 743.632f};

	vec2_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 1.0f);
	ASSERT_FLOAT_APPROX_EQ(vec2_length(p), 1.0f);
}
END_TEST

START_TEST(test_vec2_projection_01)
{
	VEC2 p = {4.0f, 3.0f};
	VEC2 q;
	VEC2 p_onto_q;

	q.x = scalar_sqrt(2.0f) / 2.0f;
	q.y = scalar_sqrt(2.0f) / 2.0f;
	vec2_projection(&p_onto_q, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(p_onto_q.x, 3.5f);
	ASSERT_FLOAT_APPROX_EQ(p_onto_q.y, 3.5f);
}
END_TEST

START_TEST(test_vec2_scale_01)
{
	VEC2 p = {4.0f, -7.0f};

	vec2_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
}
END_TEST

START_TEST(test_vec2_sub_01)
{
	VEC2 p = {3.0f, 10.0f};
	VEC2 q = {8.0f, -7.0f};
	VEC2 dest;

	vec2_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
}
END_TEST

START_TEST(test_vec2_sub_02)
{
	VEC2 p = {4.0f, 5.0f};
	VEC2 q = {-4.0f, -5.0f};
	VEC2 dest;

	vec2_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
}
END_TEST

START_TEST(test_vec3_add_01)
{
	VEC3 p = {7.0f, -2.0f, -3.0f};
	VEC3 q = {6.0f, 6.0f, -4.0f};
	VEC3 dest;

	vec3_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 13.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 4.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -7.0f);
}
END_TEST

START_TEST(test_vec3_add_02)
{
	VEC3 p = {2.0f, 9.0f, -1.0f};
	VEC3 q = {-2.0f, -9.0f, 1.0f};
	VEC3 dest;

	vec3_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 0.0f);
}
END_TEST

START_TEST(test_vec3_cross_product_01)
{
	VEC3 x_axis = {1.0f, 0.0f, 0.0f};
	VEC3 y_axis = {0.0f, 1.0f, 0.0f};
	VEC3 z_axis;

	vec3_cross(&z_axis, &x_axis, &y_axis);

	ASSERT_FLOAT_APPROX_EQ(z_axis.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(z_axis.y, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(z_axis.z, 1.0f);
}
END_TEST

START_TEST(test_vec3_cross_product_02)
{
	VEC3 p = {2.0f, 2.0f, 1.0f};
	VEC3 q = {1.0f, -2.0f, 0.0f};
	VEC3 dest;

	vec3_cross(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 2.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 1.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -6.0f);
}
END_TEST

START_TEST(test_vec3_distance_01)
{
	VEC3 p = {3.0f, 10.0f, 7.0f};
	VEC3 q = {8.0f, -7.0f, 4.0f};
	scalar distance;

	distance = vec3_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 17.972200f);
}
END_TEST

START_TEST(test_vec3_distance_02)
{
	VEC3 p = {-2.0f, -4.0f, 9.0f};
	VEC3 q = {6.0f, -7.0f, 9.5f};
	scalar distance;

	distance = vec3_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 8.558621f);
}
END_TEST

START_TEST(test_vec3_dot_product_01)
{
	VEC3 p = {2.0f, 2.0f, 1.0f};
	VEC3 q = {1.0f, -2.0f, 0.0f};
	scalar result;

	result = vec3_dot(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(result, -2.0f);
}
END_TEST

START_TEST(test_vec3_length_01)
{
	VEC3 p = {8.0f, -3.0f, 0.5f};
	scalar length;

	length = vec3_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 8.558621384311845f);
}
END_TEST

START_TEST(test_vec3_normalize_01)
{
	VEC3 p = {8.0f, -3.0f, 0.5f};

	vec3_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.934730f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -0.350524f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.058421f);
	ASSERT_FLOAT_APPROX_EQ(vec3_length(p), 1.0f);
}
END_TEST

START_TEST(test_vec3_normalize_02)
{
	VEC3 p = {-12.0f, 3.0f, -4.0f};

	vec3_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, -0.923077f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.23077f);
	ASSERT_FLOAT_APPROX_EQ(p.z, -0.307692f);
	ASSERT_FLOAT_APPROX_EQ(vec3_length(p), 1.0f);
}
END_TEST

START_TEST(test_vec3_projection_01)
{
	VEC3 p = {4.0f, 3.0f, -1.0f};
	VEC3 q;
	VEC3 p_onto_q;

	q.x = scalar_sqrt(2.0f) / 2.0f;
	q.y = scalar_sqrt(2.0f) / 2.0f;
	q.z = 0.0f;
	vec3_projection(&p_onto_q, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(p_onto_q.x, 3.5f);
	ASSERT_FLOAT_APPROX_EQ(p_onto_q.y, 3.5f);
	ASSERT_FLOAT_APPROX_EQ(p_onto_q.z, 0.0f);
}
END_TEST

START_TEST(test_vec3_scale_01)
{
	VEC3 p = {4.0f, -7.0f, 0.0f};

	vec3_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.0f);
}
END_TEST

START_TEST(test_vec3_sub_01)
{
	VEC3 p = {3.0f, 10.0f, 7.0f};
	VEC3 q = {8.0f, -7.0f, 4.0f};
	VEC3 dest;

	vec3_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 3.0f);
}
END_TEST

START_TEST(test_vec3_sub_02)
{
	VEC3 p = {4.0f, 5.0f, -11.0f};
	VEC3 q = {-4.0f, -5.0f, 11.0f};
	VEC3 dest;

	vec3_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -22.0f);
}
END_TEST

START_TEST(test_vec4_add_01)
{
	VEC4 p = {7.0f, -2.0f, -3.0f, -8.2f};
	VEC4 q = {6.0f, 6.0f, -4.0f, -7.11f};
	VEC4 dest;

	vec4_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 13.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 4.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -7.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, -15.309999f);
}
END_TEST

START_TEST(test_vec4_add_02)
{
	VEC4 p = {2.0f, 9.0f, -1.0f, -2.5f};
	VEC4 q = {-2.0f, -9.0f, 1.0f, 23.0f};
	VEC4 dest;

	vec4_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, 20.5f);
}
END_TEST

START_TEST(test_vec4_distance_01)
{
	VEC4 p = {4.0f, -4.0f, -4.0f, 4.0f};
	VEC4 q = {-6.0f, 6.0f, 6.0f, -6.0f};
	scalar distance;

	distance = vec4_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 20.0f);
}
END_TEST

START_TEST(test_vec4_dot_product_01)
{
	VEC4 p = {2.0f, 2.0f, 1.0f, 3.5f};
	VEC4 q = {1.0f, -2.0f, 0.0f, 12.0f};
	scalar result;

	result = vec4_dot(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(result, 40.0f);
}
END_TEST

START_TEST(test_vec4_length_01)
{
	VEC4 p = {8.0f, -3.0f, 0.5f, 2.5f};
	scalar length;

	length = vec4_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 8.91627725f);
}
END_TEST

START_TEST(test_vec4_normalize_01)
{
	VEC4 p = {1.0f, 1.0f, 1.0f, 1.0f};

	vec4_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(p.w, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(vec4_length(p), 1.0f);
}
END_TEST

START_TEST(test_vec4_scale_01)
{
	VEC4 p = {4.0f, -7.0f, 1.0f, -2.5};

	vec4_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 3.0f);
	ASSERT_FLOAT_APPROX_EQ(p.w, -7.5f);
}
END_TEST

START_TEST(test_vec4_sub_01)
{
	VEC4 p = {3.0f, 10.0f, 7.0f, 23.0f};
	VEC4 q = {8.0f, -7.0f, 4.0f, 12.0f};
	VEC4 dest;

	vec4_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 3.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, 11.0f);
}
END_TEST

START_TEST(test_vec4_sub_02)
{
	VEC4 p = {4.0f, 5.0f, -11.0f, -17.0f};
	VEC4 q = {-4.0f, -5.0f, 11.0f, 9.5f};
	VEC4 dest;

	vec4_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -22.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, -26.5f);
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

	test_case = tcase_create("vector sizes");
	tcase_add_test(test_case, test_vector_sizes);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 addition");
	tcase_add_test(test_case, test_vec2_add_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 distance");
	tcase_add_test(test_case, test_vec2_distance_01);
	tcase_add_test(test_case, test_vec2_distance_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 dot product");
	tcase_add_test(test_case, test_vec2_dot_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 length");
	tcase_add_test(test_case, test_vec2_length_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 normalize");
	tcase_add_test(test_case, test_vec2_normalize_01);
	tcase_add_test(test_case, test_vec2_normalize_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 projection");
	tcase_add_test(test_case, test_vec2_projection_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 scale");
	tcase_add_test(test_case, test_vec2_scale_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec2 subtraction");
	tcase_add_test(test_case, test_vec2_sub_01);
	tcase_add_test(test_case, test_vec2_sub_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 addition");
	tcase_add_test(test_case, test_vec3_add_01);
	tcase_add_test(test_case, test_vec3_add_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 cross product");
	tcase_add_test(test_case, test_vec3_cross_product_01);
	tcase_add_test(test_case, test_vec3_cross_product_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 distance");
	tcase_add_test(test_case, test_vec3_distance_01);
	tcase_add_test(test_case, test_vec3_distance_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 dot product");
	tcase_add_test(test_case, test_vec3_dot_product_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 length");
	tcase_add_test(test_case, test_vec3_length_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 normalize");
	tcase_add_test(test_case, test_vec3_normalize_01);
	tcase_add_test(test_case, test_vec3_normalize_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 projection");
	tcase_add_test(test_case, test_vec3_projection_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 scale");
	tcase_add_test(test_case, test_vec3_scale_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec3 subtraction");
	tcase_add_test(test_case, test_vec3_sub_01);
	tcase_add_test(test_case, test_vec3_sub_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 addition");
	tcase_add_test(test_case, test_vec4_add_01);
	tcase_add_test(test_case, test_vec4_add_02);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 distance");
	tcase_add_test(test_case, test_vec4_distance_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 dot product");
	tcase_add_test(test_case, test_vec4_dot_product_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 length");
	tcase_add_test(test_case, test_vec4_length_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 scale");
	tcase_add_test(test_case, test_vec4_scale_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 normalize");
	tcase_add_test(test_case, test_vec4_normalize_01);
	suite_add_tcase(suite, test_case);

	test_case = tcase_create("vec4 subtraction");
	tcase_add_test(test_case, test_vec4_sub_01);
	tcase_add_test(test_case, test_vec4_sub_02);
	suite_add_tcase(suite, test_case);

	suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
