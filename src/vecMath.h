#ifndef VECMATH_H_
#define VECMATH_H_

#include <math.h>
#include <raylib.h>
#include <stdint.h>

typedef struct ivec2 {
	int16_t x;
	int16_t y;
} ivec2_t;

typedef struct imat22 {
	int16_t a11;
	int16_t a12;
	int16_t a21;
	int16_t a22;
} imat22_t;

// common vector operations
extern float fcross2(ivec2_t a, ivec2_t b);
extern float fdot2(ivec2_t a, ivec2_t b);
extern float fdist2(ivec2_t a, ivec2_t b);

extern Vector2 ivec2ToVector2(ivec2_t v);
extern Vector2 ivec2ToScreenspace(ivec2_t v, uint8_t grid_cell);

#endif // VECMATH_H_
