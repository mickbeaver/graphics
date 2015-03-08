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

VEC2 *vec2_add(VEC2 *dest, const VEC2 *a, const VEC2 *b);

#endif /* GRAPHICS_MATH_VECTOR_H */
