#ifndef VECMATH_H_
#define VECMATH_H_

#include <math.h>

typedef struct ivec2 {
  int x;
  int y;
} ivec2_t;

typedef struct imat22 {
  int a11;
  int a12;
  int a21;
  int a22;
} imat22_t;

// common vector operations
extern float fcross2(ivec2_t a, ivec2_t b);
extern float fdot2(ivec2_t a, ivec2_t b);
extern float fdist2(ivec2_t a, ivec2_t b);

#endif // VECMATH_H_
