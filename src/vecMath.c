#include "vecMath.h"

float fcross2(ivec2_t a, ivec2_t b) { return (float)(a.x * b.y - a.y * b.x); }
float fdot2(ivec2_t a, ivec2_t b) { return (float)(a.x * b.x + a.y * b.y); }

float fdist2(ivec2_t a, ivec2_t b)
{
	return sqrtf(powf((float)(a.x - b.x), 2) + powf((float)(a.y - b.y), 2));
}

Vector2 ivec2ToVector2(ivec2_t v)
{
	return (Vector2) {.x = (float)v.x, .y = (float)v.y};
}

Vector2 ivec2ToScreenspace(ivec2_t v, uint8_t grid_cell)
{
	Vector2 res;
	res.x = (float)v.x * grid_cell;
	res.y = (float)v.y * grid_cell;
	return res;
}
