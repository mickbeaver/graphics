#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include "vector.h"

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

	ASSERT_FLOAT_EQ(dest.x, 2.0f);
	ASSERT_FLOAT_EQ(dest.y, 2.0f);
}
END_TEST

START_TEST(test_vec2_dot_01)
{
	VEC2 p, q;
	scalar dot_product;

	p = (VEC2){.x=2.0f, .y=1.25f};
	q = (VEC2){.x=1.0f, .y=-3.0f};
	dot_product = vec2_dot(&p, &q);

	ASSERT_FLOAT_EQ(dot_product, -1.75f);
}
END_TEST

START_TEST(test_vec2_length_01)
{
	VEC2 p;
	scalar length;

	p = (VEC2){.x=-12.0f, .y=5.0f};
	length = vec2_length(&p);

	ASSERT_FLOAT_EQ(length, 13.0f); 
}
END_TEST

START_TEST(test_vec2_normalize_01)
{
	VEC2 p;

	p = (VEC2){.x=12.0f, .y=5.0f};
	vec2_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.923077f, TEST_FLOAT_EPSILON); 
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.384615f, TEST_FLOAT_EPSILON); 
}
END_TEST

START_TEST(test_vec2_normalize_02)
{
	VEC2 p;

	p = (VEC2){.x=0.0f, .y=743.632f};
	vec2_normalize(&p, &p);

	ASSERT_FLOAT_EQ(p.x, 0.0f); 
	ASSERT_FLOAT_EQ(p.y, 1.0f); 
}
END_TEST

START_TEST(test_vec2_scale_01)
{
	VEC2 p;

	p = (VEC2){.x=4.0f, .y=-7.0f};
	vec2_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_EQ(p.x, 12.0f); 
	ASSERT_FLOAT_EQ(p.y, -21.0f); 
}
END_TEST

START_TEST(test_vec2_sub_01)
{
	VEC2 p, q, dest;

	p = (VEC2){.x=3.0f, .y=10.0f};
	q = (VEC2){.x=8.0f, .y=-7.0f};
	vec2_sub(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, -5.0f); 
	ASSERT_FLOAT_EQ(dest.y, 17.0f); 
}
END_TEST

START_TEST(test_vec2_sub_02)
{
	VEC2 p, q, dest;

	p = (VEC2){.x=4.0f, .y=5.0f};
	q = (VEC2){.x=-4.0f, .y=-5.0f};
	vec2_sub(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, 8.0f); 
	ASSERT_FLOAT_EQ(dest.y, 10.0f); 
}
END_TEST

START_TEST(test_vec3_add_01)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=7.0f, .y=-2.0f, .z=-3.0f};
	q = (VEC3){.x=6.0f, .y=6.0f, .z=-4.0f};
	vec3_add(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, 13.0f);
	ASSERT_FLOAT_EQ(dest.y, 4.0f);
	ASSERT_FLOAT_EQ(dest.z, -7.0f);
}
END_TEST

START_TEST(test_vec3_add_02)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=2.0f, .y=9.0f, .z=-1.0f};
	q = (VEC3){.x=-2.0f, .y=-9.0f, .z=1.0f};
	vec3_add(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, 0.0f);
	ASSERT_FLOAT_EQ(dest.y, 0.0f);
	ASSERT_FLOAT_EQ(dest.z, 0.0f);
}
END_TEST

START_TEST(test_vec3_cross_product_01)
{
	VEC3 x_axis, y_axis, z_axis;

	x_axis = (VEC3){.x=1.0f, .y=0.0f, .z=0.0f};
	y_axis = (VEC3){.x=0.0f, .y=1.0f, .z=0.0f};
	vec3_cross(&z_axis, &x_axis, &y_axis);

	ASSERT_FLOAT_EQ(z_axis.x, 0.0f);
	ASSERT_FLOAT_EQ(z_axis.y, 0.0f);
	ASSERT_FLOAT_EQ(z_axis.z, 1.0f);
}
END_TEST

START_TEST(test_vec3_cross_product_02)
{
	VEC3 p, q, dest;

	p = (VEC3){.x=2.0f, .y=2.0f, .z=1.0f};
	q = (VEC3){.x=1.0f, .y=-2.0f, .z=0.0f};
	vec3_cross(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, 2.0f);
	ASSERT_FLOAT_EQ(dest.y, 1.0f);
	ASSERT_FLOAT_EQ(dest.z, -6.0f);
}
END_TEST

START_TEST(test_vec3_dot_product_01)
{
	VEC3 p, q;
	scalar result;
	
	p = (VEC3){.x=2.0f, .y=2.0f, .z=1.0f};
	q = (VEC3){.x=1.0f, .y=-2.0f, .z=0.0f};
	result = vec3_dot(&p, &q);

	ASSERT_FLOAT_EQ(result, -2.0f);
}
END_TEST

START_TEST(test_vec3_length_01)
{
	VEC3 p;
	scalar length;

	p = (VEC3){.x=8.0f, .y=-3.0f, .z=0.5f};
	length = vec3_length(&p);

	ASSERT_FLOAT_EQ(length, 8.558621384311845f); 
}
END_TEST

START_TEST(test_vec3_normalize_01)
{
	VEC3 p;

	p = (VEC3){.x=8.0f, .y=-3.0f, .z=0.5f};
	vec3_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, 0.934730f, TEST_FLOAT_EPSILON); 
	ASSERT_FLOAT_APPROX_EQ(p.y, -0.350524f, TEST_FLOAT_EPSILON); 
	ASSERT_FLOAT_APPROX_EQ(p.z, 0.058421f, TEST_FLOAT_EPSILON); 
}
END_TEST

START_TEST(test_vec3_normalize_02)
{
	VEC3 p;

	p = (VEC3){.x=-12.0f, .y=3.0f, .z=-4.0f};
	vec3_normalize(&p, &p);

	ASSERT_FLOAT_APPROX_EQ(p.x, -0.92308f, TEST_FLOAT_EPSILON); 
	ASSERT_FLOAT_APPROX_EQ(p.y, 0.23077f, TEST_FLOAT_EPSILON); 
	ASSERT_FLOAT_APPROX_EQ(p.z, -0.30769f, TEST_FLOAT_EPSILON); 
}
END_TEST

START_TEST(test_vec3_scale_01)
{
	VEC3 p;

	p = (VEC3){.x=4.0f, .y=-7.0f, .z=0.0f};
	vec3_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_EQ(p.x, 12.0f); 
	ASSERT_FLOAT_EQ(p.y, -21.0f); 
	ASSERT_FLOAT_EQ(p.z, 0.0f); 
}
END_TEST

START_TEST(test_vec4_add_01)
{
	VEC4 p, q, dest;

	p = (VEC4){.x=7.0f, .y=-2.0f, .z=-3.0f, .w=-8.2f};
	q = (VEC4){.x=6.0f, .y=6.0f, .z=-4.0f, .w=-7.11f};
	vec4_add(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, 13.0f);
	ASSERT_FLOAT_EQ(dest.y, 4.0f);
	ASSERT_FLOAT_EQ(dest.z, -7.0f);
	ASSERT_FLOAT_EQ(dest.w, -15.309999f);
}
END_TEST

START_TEST(test_vec4_add_02)
{
	VEC4 p, q, dest;

	p = (VEC4){.x=2.0f, .y=9.0f, .z=-1.0f, .w=-2.5f};
	q = (VEC4){.x=-2.0f, .y=-9.0f, .z=1.0f, .w=23.0f};
	vec4_add(&dest, &p, &q);

	ASSERT_FLOAT_EQ(dest.x, 0.0f);
	ASSERT_FLOAT_EQ(dest.y, 0.0f);
	ASSERT_FLOAT_EQ(dest.z, 0.0f);
	ASSERT_FLOAT_EQ(dest.w, 20.5f);
}
END_TEST

START_TEST(test_vec4_dot_product_01)
{
	VEC4 p, q;
	scalar result;
	
	p = (VEC4){.x=2.0f, .y=2.0f, .z=1.0f, .w=3.5f};
	q = (VEC4){.x=1.0f, .y=-2.0f, .z=0.0f, .w=12.0f};
	result = vec4_dot(&p, &q);

	ASSERT_FLOAT_EQ(result, 40.0f);
}
END_TEST

START_TEST(test_vec4_length_01)
{
	VEC4 p;
	scalar length;

	p = (VEC4){.x=8.0f, .y=-3.0f, .z=0.5f, .w=2.5f};
	length = vec4_length(&p);

	ASSERT_FLOAT_APPROX_EQ(length, 8.91627725f, TEST_FLOAT_EPSILON); 
}
END_TEST

START_TEST(test_vec4_normalize_01)
{
	VEC4 p;

	p = (VEC4){.x=1.0f, .y=1.0f, .z=1.0f, .w=1.0f};
	vec4_normalize(&p, &p);

	ASSERT_FLOAT_EQ(p.x, 0.5f);
	ASSERT_FLOAT_EQ(p.y, 0.5f);
	ASSERT_FLOAT_EQ(p.z, 0.5f);
	ASSERT_FLOAT_EQ(p.w, 0.5f);
}
END_TEST

START_TEST(test_vec4_scale_01)
{
	VEC4 p;

	p = (VEC4){.x=4.0f, .y=-7.0f, .z=1.0f, .w=-2.5};
	vec4_scale(&p, 3.0f, &p);

	ASSERT_FLOAT_EQ(p.x, 12.0f); 
	ASSERT_FLOAT_EQ(p.y, -21.0f); 
	ASSERT_FLOAT_EQ(p.z, 3.0f);
	ASSERT_FLOAT_EQ(p.w, -7.5f);
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

	test_case = tcase_create("vec2 scale");
	tcase_add_test(test_case, test_vec2_scale_01);
	suite_add_tcase(suite, test_case);
	
	test_case = tcase_create("vec3 subtraction");
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

	test_case = tcase_create("vec3 scale");
	tcase_add_test(test_case, test_vec3_scale_01);
	suite_add_tcase(suite, test_case);
	
	test_case = tcase_create("vec4 addition");
	tcase_add_test(test_case, test_vec4_add_01);
	tcase_add_test(test_case, test_vec4_add_02);
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

	suite_runner = srunner_create(suite);
	srunner_run_all(suite_runner, CK_NORMAL);
	number_failed = srunner_ntests_failed(suite_runner);
	srunner_free(suite_runner);

	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
