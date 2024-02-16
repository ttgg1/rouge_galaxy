#ifndef VECMATH_H_
#define VECMATH_H_

#include <math.h>
#include <stdint.h>

typedef struct ivec2 {
  int8_t x;
  int8_t y;
} ivec2_t;

typedef struct imat22 {
  int8_t a11;
  int8_t a12;
  int8_t a21;
  int8_t a22;
} imat22_t;

// common vector operations
extern float fcross2(ivec2_t a, ivec2_t b);
extern float fdot2(ivec2_t a, ivec2_t b);
extern float fdist2(ivec2_t a, ivec2_t b);

#endif // VECMATH_H_
