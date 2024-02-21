#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include <stdint.h>

typedef struct quadTree {
    struct quadTree *nw;
    struct quadTree *ne;
    struct quadTree *sw;
    struct quadTree *se;
    uint32_t value;
} quadTree_t;


typedef struct map{
    quadTree_t *t;
    int height;
} map_t;



quadTree_t *qt_create();
void qt_destroy(quadTree_t *t);

bool qt_isLeaf(quadTree_t *t);

map_t *m_create();
void m_destroy(map_t *m);

bool m_inBounds(map_t *m, int y, int x);
bool m_inBoundsNW(map_t *m, int y, int x);
bool m_inBoundsNE(map_t *m, int y, int x);
bool m_inBoundsSW(map_t *m, int y, int x);
bool m_inBoundsSE(map_t *m, int y, int x);

quadTree_t *m_getContainingTree(map_t *m, int y, int x, int *h, int *offY, int *offX);

void m_setAt(map_t *m, int y, int x, uint32_t c);
uint32_t m_getAt(map_t *m, int y, int x);

#endif // MAP_H