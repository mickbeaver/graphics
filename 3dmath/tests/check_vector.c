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
	VEC2 p, q, dest;

	p = (VEC2){.x=1.0f, .y=1.0f};
	q = (VEC2){.x=1.0f, .y=1.0f};
	vec2_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 2.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 2.0f);
}
END_TEST

START_TEST(test_vec2_distance_01)
{
	VEC2 p, q;
	scalar distance;

	p = (VEC2){.x=10.0f, .y=6.0f};
	q = (VEC2){.x=-14.0f, .y=30.0f};
	distance = vec2_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 33.941124f);
}
END_TEST

START_TEST(test_vec2_distance_02)
{
	VEC2 p, q;
	scalar distance;

	p = (VEC2){.x=0.0f, .y=0.0f};
	q = (VEC2){.x=-12.0f, .y=5.0f};
	distance = vec2_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 13.0f);
}
END_TEST

START_TEST(test_vec2_dot_01)
{
	VEC2 p, q;
	scalar dot_product;

	p = (VEC2){.x=2.0f, .y=1.25f};
	q = (VEC2){.x=1.0f, .y=-3.0f};
	dot_product = vec2_dot(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(dot_product, -1.75f);
}
END_TEST

START_TEST(test_vec2_length_01)
{
	VEC2 p;
	scalar length;

	p = (VEC2){.x=-12.0f, .y=5.0f};
	length = vec2_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 13.0f);
}
END_TEST

START_TEST(test_vec2_normalize_01)
{
	VEC2 p;

	p = (VEC2){.x=12.0f, .y=5.0f};
	vec2_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.923077f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.384615f);
}
END_TEST

START_TEST(test_vec2_normalize_02)
{
	VEC2 p;

	p = (VEC2){.x=0.0f, .y=743.632f};
	vec2_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 1.0f);
}
END_TEST

START_TEST(test_vec2_projection_01)
{
	VEC2 p, q, p_onto_q;

	p = (VEC2){.x=4.0f, .y=3.0f};
	q.x = scalar_sqrt(2.0f) / 2.0f;
	q.y = scalar_sqrt(2.0f) / 2.0f;
	vec2_projection(&p_onto_q, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(p_onto_q.x, 3.5f);
	ASSERT_FLOAT_APPROX_EQ(p_onto_q.y, 3.5f);
}
END_TEST

START_TEST(test_vec2_scale_01)
{
	VEC2 p;

	p = (VEC2){.x=4.0f, .y=-7.0f};
	vec2_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
}
END_TEST

START_TEST(test_vec2_sub_01)
{
	VEC2 p, q, dest;

	p = (VEC2){.x=3.0f, .y=10.0f};
	q = (VEC2){.x=8.0f, .y=-7.0f};
	vec2_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
}
END_TEST

START_TEST(test_vec2_sub_02)
{
	VEC2 p, q, dest;

	p = (VEC2){.x=4.0f, .y=5.0f};
	q = (VEC2){.x=-4.0f, .y=-5.0f};
	vec2_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
}
END_TEST

START_TEST(test_vec3_add_01)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=7.0f, .y=-2.0f, .z=-3.0f};
	q = (VEC3){.x=6.0f, .y=6.0f, .z=-4.0f};
	vec3_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 13.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 4.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -7.0f);
}
END_TEST

START_TEST(test_vec3_add_02)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=2.0f, .y=9.0f, .z=-1.0f};
	q = (VEC3){.x=-2.0f, .y=-9.0f, .z=1.0f};
	vec3_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 0.0f);
}
END_TEST

START_TEST(test_vec3_cross_product_01)
{
	VEC3 x_axis, y_axis, z_axis;

	x_axis = (VEC3){.x=1.0f, .y=0.0f, .z=0.0f};
	y_axis = (VEC3){.x=0.0f, .y=1.0f, .z=0.0f};
	vec3_cross(&z_axis, &x_axis, &y_axis);

	ASSERT_FLOAT_APPROX_EQ(z_axis.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(z_axis.y, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(z_axis.z, 1.0f);
}
END_TEST

START_TEST(test_vec3_cross_product_02)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=2.0f, .y=2.0f, .z=1.0f};
	q = (VEC3){.x=1.0f, .y=-2.0f, .z=0.0f};
	vec3_cross(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 2.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 1.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -6.0f);
}
END_TEST

START_TEST(test_vec3_distance_01)
{
	VEC3 p, q;
	scalar distance;

	p = (VEC3){.x=3.0f, .y=10.0f, .z=7.0f};
	q = (VEC3){.x=8.0f, .y=-7.0f, .z=4.0f};
	distance = vec3_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 17.972200f);
}
END_TEST

START_TEST(test_vec3_distance_02)
{
	VEC3 p, q;
	scalar distance;

	p = (VEC3){.x=-2.0f, .y=-4.0f, .z=9.0f};
	q = (VEC3){.x=6.0f, .y=-7.0f, .z=9.5f};
	distance = vec3_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 8.558621f);
}
END_TEST

START_TEST(test_vec3_dot_product_01)
{
	VEC3 p, q;
	scalar result;

	p = (VEC3){.x=2.0f, .y=2.0f, .z=1.0f};
	q = (VEC3){.x=1.0f, .y=-2.0f, .z=0.0f};
	result = vec3_dot(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(result, -2.0f);
}
END_TEST

START_TEST(test_vec3_length_01)
{
	VEC3 p;
	scalar length;

	p = (VEC3){.x=8.0f, .y=-3.0f, .z=0.5f};
	length = vec3_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 8.558621384311845f);
}
END_TEST

START_TEST(test_vec3_normalize_01)
{
	VEC3 p;

	p = (VEC3){.x=8.0f, .y=-3.0f, .z=0.5f};
	vec3_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.934730f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -0.350524f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.058421f);
}
END_TEST

START_TEST(test_vec3_normalize_02)
{
	VEC3 p;

	p = (VEC3){.x=-12.0f, .y=3.0f, .z=-4.0f};
	vec3_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, -0.923077f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.23077f);
	ASSERT_FLOAT_APPROX_EQ(p.z, -0.307692f);
}
END_TEST

START_TEST(test_vec3_projection_01)
{
	VEC3 p, q, p_onto_q;

	p = (VEC3){.x=4.0f, .y=3.0f, .z=-1.0f};
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
	VEC3 p;

	p = (VEC3){.x=4.0f, .y=-7.0f, .z=0.0f};
	vec3_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.0f);
}
END_TEST

START_TEST(test_vec3_sub_01)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=3.0f, .y=10.0f, .z=7.0f};
	q = (VEC3){.x=8.0f, .y=-7.0f, .z=4.0f};
	vec3_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 3.0f);
}
END_TEST

START_TEST(test_vec3_sub_02)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=4.0f, .y=5.0f, .z=-11.0f};
	q = (VEC3){.x=-4.0f, .y=-5.0f, .z=11.0f};
	vec3_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 8.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 10.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -22.0f);
}
END_TEST

START_TEST(test_vec4_add_01)
{
	VEC4 p, q, dest;

	p = (VEC4){.x=7.0f, .y=-2.0f, .z=-3.0f, .w=-8.2f};
	q = (VEC4){.x=6.0f, .y=6.0f, .z=-4.0f, .w=-7.11f};
	vec4_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 13.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 4.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, -7.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, -15.309999f);
}
END_TEST

START_TEST(test_vec4_add_02)
{
	VEC4 p, q, dest;

	p = (VEC4){.x=2.0f, .y=9.0f, .z=-1.0f, .w=-2.5f};
	q = (VEC4){.x=-2.0f, .y=-9.0f, .z=1.0f, .w=23.0f};
	vec4_add(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 0.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, 20.5f);
}
END_TEST

START_TEST(test_vec4_distance_01)
{
	VEC4 p, q;
	scalar distance;

	p = (VEC4){.x=4.0f, .y=-4.0f, .z=-4.0f, .w=4.0f};
	q = (VEC4){.x=-6.0f, .y=6.0f, .z=6.0f, .w=-6.0f};
	distance = vec4_distance(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(distance, 20.0f);
}
END_TEST

START_TEST(test_vec4_dot_product_01)
{
	VEC4 p, q;
	scalar result;

	p = (VEC4){.x=2.0f, .y=2.0f, .z=1.0f, .w=3.5f};
	q = (VEC4){.x=1.0f, .y=-2.0f, .z=0.0f, .w=12.0f};
	result = vec4_dot(&p, &q);

	ASSERT_FLOAT_APPROX_EQ(result, 40.0f);
}
END_TEST

START_TEST(test_vec4_length_01)
{
	VEC4 p;
	scalar length;

	p = (VEC4){.x=8.0f, .y=-3.0f, .z=0.5f, .w=2.5f};
	length = vec4_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 8.91627725f);
}
END_TEST

START_TEST(test_vec4_normalize_01)
{
	VEC4 p;

	p = (VEC4){.x=1.0f, .y=1.0f, .z=1.0f, .w=1.0f};
	vec4_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.5f);
	ASSERT_FLOAT_APPROX_EQ(p.w, 0.5f);
}
END_TEST

START_TEST(test_vec4_scale_01)
{
	VEC4 p;

	p = (VEC4){.x=4.0f, .y=-7.0f, .z=1.0f, .w=-2.5};
	vec4_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 12.0f);
	ASSERT_FLOAT_APPROX_EQ(p.y, -21.0f);
	ASSERT_FLOAT_APPROX_EQ(p.z, 3.0f);
	ASSERT_FLOAT_APPROX_EQ(p.w, -7.5f);
}
END_TEST

START_TEST(test_vec4_sub_01)
{
	VEC4 p, q, dest;

	p = (VEC4){.x=3.0f, .y=10.0f, .z=7.0f, .w=23.0f};
	q = (VEC4){.x=8.0f, .y=-7.0f, .z=4.0f, .w=12.0f};
	vec4_sub(&dest, &p, &q);

	ASSERT_FLOAT_APPROX_EQ(dest.x, -5.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.y, 17.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.z, 3.0f);
	ASSERT_FLOAT_APPROX_EQ(dest.w, 11.0f);
}
END_TEST

START_TEST(test_vec4_sub_02)
{
	VEC4 p, q, dest;

	p = (VEC4){.x=4.0f, .y=5.0f, .z=-11.0f, .w=-17.0f};
	q = (VEC4){.x=-4.0f, .y=-5.0f, .z=11.0f, .w=9.5f};
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
