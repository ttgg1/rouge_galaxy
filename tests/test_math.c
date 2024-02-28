#include "test_main.h"

bool test_math_fcross2(void) {
  printf("testing math function fcross2 ");
  ivec2_t a = {1, 2};
  ivec2_t b = {-1, -187};

  float cross = fcross2(a, b);

  return CHECK_EQUAL_FLOAT(-185.0f, cross);
}

bool test_math_fdot2(void) {
  printf("testing math function fdot2 ");
  ivec2_t a = {1, 2};
  ivec2_t b = {-1, -187};

  float dot = fdot2(a, b);

  return CHECK_EQUAL_FLOAT(-375.0f, dot);
}

bool test_math_fdist2(void) {
  printf("testing math function fdist2 ");
  ivec2_t a = {1, 2};
  ivec2_t b = {-1, -187};

  float dist = fdist2(a, b);

  return CHECK_EQUAL_FLOAT(189.010574f, dist);
}
