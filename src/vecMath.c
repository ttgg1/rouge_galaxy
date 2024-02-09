#include "vecMath.h"

float fcross2(ivec2_t a, ivec2_t b) { return (float)a.x * b.y - a.y * b.x; }
float fdot2(ivec2_t a, ivec2_t b) { return (float)a.x * b.x + a.y * b.y; }

float fdist2(ivec2_t a, ivec2_t b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
