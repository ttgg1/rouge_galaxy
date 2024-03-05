#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include <stdint.h>

#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "vecMath.h"

#define NUM_CONSTRAINTS 2
#define NUM_TILES 2

typedef struct quadTree {
    struct quadTree *nw;
    struct quadTree *ne;
    struct quadTree *sw;
    struct quadTree *se;
    uint32_t value;
    bool hasCollapsed;
    bool superposition[NUM_CONSTRAINTS];
    int numSuperpos;
} quadTree_t;


typedef struct map{
    quadTree_t *t;
    int height;
    bool constraints[NUM_CONSTRAINTS * NUM_CONSTRAINTS * 4]; 
    uint32_t tileset[NUM_TILES];
} map_t;


typedef struct wfcdata{
    int32_t yStart;
    int32_t yEnd;
    int32_t xStart;
    int32_t xEnd;
    int tilesLeft;
    list_t *updates;
} wfcdata_t;


quadTree_t *qt_create();
void qt_destroy(quadTree_t *t);

bool qt_isLeaf(quadTree_t* t);

map_t* m_create();
void m_destroy(map_t* m);

bool m_inBounds(int height, int y, int x);
bool m_inBoundsNW(int height, int y, int x);
bool m_inBoundsNE(int height, int y, int x);
bool m_inBoundsSW(int height, int y, int x);
bool m_inBoundsSE(int height, int y, int x);

quadTree_t *m_getContainingTree(map_t *m, quadTree_t *current, int y, int x, int *h, int *offY, int *offX);

void m_collapseTree(quadTree_t *t, int h);

void m_expand(map_t *m);

void m_setAt(map_t *m, int y, int x, uint32_t c);
uint32_t m_getAt(map_t *m, int y, int x);

void m_generateMap(map_t *m, int yStart, int yEnd, int xStart, int xEnd);
void m_propagateChanges(map_t *m, wfcdata_t *data);

#endif // MAP_H