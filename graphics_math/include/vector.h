#ifndef GRAPHICS_MATH_VECTOR_H
#define GRAPHICS_MATH_VECTOR_H

typedef float scalar;

typedef struct vec2 {
	scalar x;
	scalar y;
} VEC2;

typedef struct vec3 {
	scalar x;
	scalar y;
	scalar z;
} VEC3;

typedef struct vec4 {
	scalar x;
	scalar y;
	scalar z;
	scalar w;
} VEC4;

VEC2	*vec2_add(VEC2 *dest, const VEC2 *p, const VEC2 *q);
scalar	 vec2_cross(const VEC2 *p, const VEC2 *q);
scalar	 vec2_dot(const VEC2 *p, const VEC2 *q);
scalar	 vec2_length(const VEC2 *p);
VEC2	*vec2_projection(VEC2 *p_onto_q, const VEC2 *p, VEC2 *q);
VEC2	*vec2_scale(VEC2 *dest, scalar scale, const VEC2 *q);
scalar	 vec2_sqlength(const VEC2 *p);
VEC2	*vec2_sub(VEC2 *dest, const VEC2 *p, const VEC2 *q);

VEC3	*vec3_add(VEC3 *dest, const VEC3 *p, const VEC3 *q);
VEC3	*vec3_cross(VEC3 *dest, const VEC3 *p, const VEC3 *q);
scalar	 vec3_dot(const VEC3 *p, const VEC3 *q);
scalar	 vec3_length(const VEC3 *p);
VEC3	*vec3_projection(VEC3 *p_onto_q, const VEC3 *p, VEC3 *q);
VEC3	*vec3_scale(VEC3 *dest, scalar scale, const VEC3 *q);
scalar	 vec3_sqlength(const VEC3 *p);
VEC3	*vec3_sub(VEC3 *dest, const VEC3 *p, const VEC3 *q);

VEC4	*vec4_add(VEC4 *dest, const VEC4 *p, const VEC4 *q);
scalar	 vec4_dot(const VEC4 *p, const VEC4 *q);
scalar	 vec4_length(const VEC4 *p);
VEC4	*vec4_projection(VEC4 *p_onto_q, const VEC4 *p, VEC4 *q);
VEC4	*vec4_scale(VEC4 *dest, scalar scale, const VEC4 *q);
scalar	 vec4_sqlength(const VEC4 *p);
VEC4	*vec4_sub(VEC4 *dest, const VEC4 *p, const VEC4 *q);

#endif /* GRAPHICS_MATH_VECTOR_H */
