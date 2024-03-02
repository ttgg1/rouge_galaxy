#include "map.h"

quadTree_t *qt_create(){
    quadTree_t *t;
    t = (quadTree_t *) malloc(sizeof(quadTree_t));
    if (t == NULL) {
        debug_print("Failed to malloc quadTree_t ! \n");
    }
    t->nw = NULL;
    t->ne = NULL;
    t->sw = NULL;
    t->se = NULL;
    t->value = NULL;
    t->hasCollapsed = NULL;
    return t;
}

void qt_destroy(quadTree_t *t){
    if (t == NULL) {
        return;
    }
    if (t->nw != NULL) {
        qt_destroy(t->nw);
    }
    if (t->ne != NULL) {
        qt_destroy(t->ne);
    }
    if (t->sw != NULL) {
        qt_destroy(t->sw);
    }
    if (t->se != NULL) {
        qt_destroy(t->se);
    }

    free(t);
}

map_t* m_create()
{
	map_t* m;
	m = (map_t*) malloc(sizeof(map_t));

map_t *m_create() {
    map_t *m;
    m = (map_t *) malloc(sizeof(map_t));
    if (m == NULL) {
        debug_print("Failed to malloc map_t ! \n");
    }
    quadTree_t *t = qt_create();
    if (t == NULL) {
        debug_print("Failed to create quadTree_t for map_t ! \n");
    }
    
    m->t = t;
    m->height = 0;
    return m;
}

void m_destroy(map_t* m)
{
	if (m == NULL) {
		return;
	}

	if (m->t != NULL) {
		qt_destroy(m->t);
	}

	free(m);
}

bool m_inBounds(map_t* m, int y, int x)
{
	printf("inBounds: y: %d, x: %d, h: %d | %d %d %d %d\n", y, x, m->height, -(1 << (m->height - 1)) + 1, (1 << (m->height - 1)),
	       -(1 << (m->height - 1)) + 1, (1 << (m->height - 1)));

	if (m->height == 0) {
		return y == 0 && x == 0;
	}

	return y >= -(1 << (m->height - 1)) + 1
	       && y <= (1 << (m->height - 1))
	       && x >= -(1 << (m->height - 1)) + 1
	       && x <= (1 << (m->height - 1));
bool m_inBounds(int height, int y, int x) {
    if (height == 0) {
        return y == 0 && x == 0;
    }
    
    return y >= -(1 << (height - 1)) + 1
        && y <= (1 << (height - 1))
        && x >= -(1 << (height - 1)) + 1
        && x <= (1 << (height - 1)); 
}

bool m_inBoundsNW(map_t* m, int y, int x)
{
	return y >= -(1 << (m->height - 1)) + 1
	       && y <= 0
	       && x >= -(1 << (m->height - 1)) + 1
	       && x <= 0;
bool m_inBoundsNW(int height, int y, int x) {
    return y >= -(1 << (height - 1)) + 1
        && y <= 0
        && x >= -(1 << (height - 1)) + 1
        && x <= 0; 
}

bool m_inBoundsNE(int height, int y, int x) {
    return y >= -(1 << (height - 1)) + 1
        && y <= 0
        && x >= 1
        && x <= (1 << (height - 1)); 
}

bool m_inBoundsSW(int height, int y, int x) {
    return y >= 1
        && y <= (1 << (height - 1))
        && x >= -(1 << (height - 1)) + 1
        && x <= 0; 
}

bool m_inBoundsSE(int height, int y, int x) {
    return y >= 1
        && y <= (1 << (height - 1))
        && x >= 1
        && x <= (1 << (height - 1)); 
}

bool qt_isLeaf(quadTree_t* t)
{
	return t->nw == NULL && t->ne == NULL && t->sw == NULL && t->se == NULL;
}

/*
    returns sub-tree which contains coordinates (x,y) as well as changing height and offset values
    passed used for translating local sub-tree coordinates to absolute values
*/
quadTree_t *m_getContainingTree(map_t *m, quadTree_t *current, int y, int x, int *height, int *offY, int *offX) {

	// offsets for translating local tree coordinates
	int offsetY, offsetX, h;
	offsetY = *offY;
	offsetX = *offX;

    h = *height;
    if (current == NULL) {
        current = m->t;
    }
        
    while (!qt_isLeaf(current)){// && h > 0) {
        
        if (m_inBoundsNW(h, y+offsetY, x+offsetX)) {

			offsetY += (1 << (h - 2));
			offsetX += (1 << (h - 2));

            current = current->nw;
            h--;
        }
        else if (m_inBoundsNE(h, y+offsetY, x+offsetX)) {

			offsetY += (1 << (h - 2));
			offsetX -= (1 << (h - 2));

            current = current->ne;
            h--;
        }
        else if (m_inBoundsSW(h, y+offsetY, x+offsetX)) {

			offsetY -= (1 << (h - 2));
			offsetX += (1 << (h - 2));

            current = current->sw;
            h--;
        }
        else if (m_inBoundsSE(h, y+offsetY, x+offsetX)) {

			offsetY -= (1 << (h - 2));
			offsetX -= (1 << (h - 2));

			current = current->se;
			h--;
		}

	}

	*height = h;
	*offY = offsetY;
	*offX = offsetX;
	return current;
}


void m_collapseTree(quadTree_t *t, int h) {
    if (h < 1) {
         
        // leaf 

        t->hasCollapsed = false;
        t->numSuperpos = NUM_CONSTRAINTS;
        static const bool temp[NUM_CONSTRAINTS] = {true, true};
        memcpy(t->superposition, temp, sizeof temp);

        return;
    }

    // create sub-trees
    t->nw = qt_create();
    t->nw->value = t->value;
    t->ne = qt_create();
    t->ne->value = t->value;
    t->sw = qt_create();
    t->sw->value = t->value;
    t->se = qt_create();
    t->se->value = t->value;
    t->value = NULL;

    // collapse subtrees
    m_collapseTree(t->nw, h-1);
    m_collapseTree(t->ne, h-1);
    m_collapseTree(t->sw, h-1);
    m_collapseTree(t->se, h-1);
}

void m_expand(map_t *m) {

    if (m->height == 0) {
        // tree is leaf
        m->height++;
        m_collapseTree(m->t, m->height);
        return;
    }
    
    // expand tree
    quadTree_t *newTree = qt_create();

    int h = m->height - 1;

    // NW
    newTree->nw = qt_create();

    newTree->nw->nw = qt_create();
    m_collapseTree(newTree->nw->nw, h);

    newTree->nw->ne = qt_create();
    m_collapseTree(newTree->nw->ne, h);

    newTree->nw->sw = qt_create();
    m_collapseTree(newTree->nw->sw, h);

    newTree->nw->se = m->t->nw;

    // NE
    newTree->ne = qt_create();

    newTree->ne->nw = qt_create();
    m_collapseTree(newTree->ne->nw, h);

    newTree->ne->ne = qt_create();
    m_collapseTree(newTree->ne->ne, h);

    newTree->ne->sw = m->t->ne;

    newTree->ne->se = qt_create(); 
    m_collapseTree(newTree->ne->se, h);

    // SW
    newTree->sw = qt_create();

    newTree->sw->nw = qt_create();
    m_collapseTree(newTree->sw->nw, h);
    
    newTree->sw->ne = m->t->sw;

    newTree->sw->sw = qt_create();
    m_collapseTree(newTree->sw->sw, h);

    newTree->sw->se = qt_create(); 
    m_collapseTree(newTree->sw->se, h);

    // SE
    newTree->se = qt_create();

    newTree->se->nw = m->t->se;

    newTree->se->ne = qt_create();
    m_collapseTree(newTree->se->ne, h);

    newTree->se->sw = qt_create();
    m_collapseTree(newTree->se->sw, h);

    newTree->se->se = qt_create(); 
    m_collapseTree(newTree->se->se, h);


    quadTree_t *temp = m->t;
    temp->nw = NULL;
    temp->ne = NULL;
    temp->sw = NULL;
    temp->se = NULL;
    m->t = newTree;
    qt_destroy(temp);
    m->height++;
}


void m_setAt(map_t *m, int y, int x, uint32_t c) {

    // offsets for translating local tree coordinates
    int offsetY, offsetX, h;
    offsetY = 0;
    offsetX = 0;

    h = m->height;

    if (m_inBounds(h, y, x)) {
        // (x,y) in initialized map area
        quadTree_t *tree = m_getContainingTree(m, NULL, y, x, &h, &offsetY, &offsetX);

        if (h == 0) {
            // containing tree is leaf, base case
            tree->value = c;
            return;
        }

        // containing tree isnt leaf
        m_collapseTree(tree, h);

        tree = m_getContainingTree(m, tree, y, x, &h, &offsetY, &offsetX);
        
        tree->value = c;
        return;
    }

    // (x,y) not in initialized area
    m_expand(m);
    m_setAt(m, y, x, c);
}

uint32_t m_getAt(map_t *m, int y, int x) {
    if (!m_inBounds(m->height, y, x)) {
        // not in initialized area
        return (uint32_t)'#';
    }
    // offsets for translating local tree coordinates
    int offsetY, offsetX, h;
    offsetY = 0;
    offsetX = 0;

    h = m->height;
    quadTree_t *tree = m_getContainingTree(m, NULL, y, x, &h, &offsetY, &offsetX);
    return tree->value;
}


int OFFSETS[] = {
    -1, 0, // NORTH
    0, 1,  // EAST
    1, 0,  // SOUTH
    0, -1  // WEST
};

int n_offsets = 4;


void m_generateMap(map_t *m, int yStart, int yEnd, int xStart, int xEnd) {
    wfcdata_t * data;
    data = (wfcdata_t *) malloc(sizeof(wfcdata_t));
    if (data == NULL) {
        return;
    }
    data->yStart = yStart;
    data->yEnd = yEnd;
    data->xStart = xStart;
    data->xEnd = xEnd;
    data->tilesLeft = (yEnd - yStart) * (xEnd - xStart);

    data->updates = li_emptyList();
    

    quadTree_t *tree, *lowestTree;
    int h, offY, offX;

    int posY, posX, lowest, collapsed;
    while (data->tilesLeft > 0) {
        lowest = NUM_CONSTRAINTS + 1;
        for (int y = yStart; y < yEnd; y++) {
            for (int x = xStart; x < xEnd; x++) {
                h = m->height;
                offY = 0;
                offX = 0;
                tree = m_getContainingTree(m, NULL, y, x, &h, &offY, &offX);
                if (tree->hasCollapsed || tree->numSuperpos < 1) {
                    continue;
                }

                if (tree->numSuperpos < lowest) {
                    lowest = tree->numSuperpos;
                    lowestTree = tree;
                    posY = y;
                    posX = x;
                }
            }
        }

        if (lowest == NUM_CONSTRAINTS + 1){
            break;
        }
        //printf("lowest: %d, ", lowest);
        collapsed = rand() % lowest;
        //printf("collapsed: %d\n", collapsed);
        for (int i = 0; i < NUM_CONSTRAINTS; i++) {
            if (lowestTree->superposition[i]) {
                if (collapsed == 0) {
                    lowestTree->hasCollapsed = true;
                    lowestTree->numSuperpos = 1;
                    lowestTree->value = m->tileset[i];
                    data->tilesLeft--;
                    //printf("collapsed %d %d to %c (%d), remaining: %d\n", posY, posX, (char)lowestTree->value, i, data->tilesLeft);
                }else{
                    lowestTree->superposition[i] = false;
                    //printf("%d, %d removed superpos %d\n", posY, posX, i);
                }
                collapsed--;
            }
        }
        li_push(data->updates, &(ivec2_t){posX, posY});
        m_propagateChanges(m, data);
    }

    li_destroy(data->updates);
    free(data);
}


void m_propagateChanges(map_t *m, wfcdata_t *data) {
    int yStart, xStart;
    
    int offsetY, offsetX, h;
    bool changes[n_offsets];
    while (data->updates->head != NULL) {
        ivec2_t *pos = (ivec2_t *)li_pop(data->updates);
        yStart = pos->y;
        xStart = pos->x;
        //printf("propagating changes from %d, %d ", yStart, xStart);
        if (yStart < data->yStart || yStart >= data->yEnd || xStart < data->xStart || xStart >= data->xEnd) {
            // return if starting tile is out-of-bounds
            //printf("out of bounds!\n");
            return;
        }
        //printf("\n");

        
        for (int i = 0; i < n_offsets; i++) {
            changes[i] = false;
        }

        // offsets for translating local tree coordinates
        
        offsetY = 0;
        offsetX = 0;
        h = m->height;

        quadTree_t *tree = m_getContainingTree(m, NULL, yStart, xStart, &h, &offsetY, &offsetX);

        // iterate over adjacent neighbours (N,E,S,W) of starting tile
        
        int posY, posX, idxSuperpos;
        quadTree_t *nTree;
        for (int direction = 0; direction < n_offsets; direction++) {
            posY = yStart + OFFSETS[direction*2];
            posX = xStart + OFFSETS[direction*2+1];

            offsetY = 0;
            offsetX = 0;
            h = m->height;

            nTree = m_getContainingTree(m, NULL, posY, posX, &h, &offsetY, &offsetX);

            //skip collapsed neighbour
            if (nTree->hasCollapsed) {
                continue;
            }

            // skip out-of-bounds
            if (posY < data->yStart || posY >= data->yEnd || posX < data->xStart || posX >= data->xEnd) {
                continue;
            }

            // determine possible tiles for neighbour 

            bool superpos[NUM_CONSTRAINTS] = {false};
            
            for (int idx_cons = 0; idx_cons < NUM_CONSTRAINTS; idx_cons++) {
                
                if (!tree->superposition[idx_cons]) {
                    continue;
                }

                // if tile is in the starting tiles allowed tileset, add its constraintset to possible tiles for neighbour
                for (int idx = 0; idx < NUM_CONSTRAINTS; idx++) {
                    
                    /*
                    if (m->constraints[idx_cons][direction][idx]) {
                        superpos[idx] = true;
                    }
                    */

                    if (m->constraints[idx_cons * NUM_CONSTRAINTS * n_offsets + direction * NUM_CONSTRAINTS + idx]) {
                        superpos[idx] = true;
                    }
                }
            }

            // update allowed tiles for neighbour
            idxSuperpos = -1;

            for (int i = 0; i < NUM_CONSTRAINTS; i++) {
                if (nTree->superposition[i]) {

                    if (superpos[i]) {
                        idxSuperpos = i;
                    }else {
                        //printf("%d, %d removed superpos %d\n", posY, posX, i);
                        nTree->superposition[i] = false;
                        nTree->numSuperpos--;
                        // mark neighbour if tile is removed from neighbours tileset
                        changes[direction] = true;
                    }
                }
            }
            // if only one possible tile for neighbour remains, collapse tile
            if (nTree->numSuperpos == 1 && !nTree->hasCollapsed) {
                nTree->hasCollapsed = true;
                nTree->value = m->tileset[idxSuperpos];
                data->tilesLeft--;
                //printf("%d %d only one remaining, collapsed to %c (%d), remaining: %d\n", posY, posX, (char)nTree->value, idxSuperpos, data->tilesLeft);
            }
        }

        // continue propagating changes to updated neighbouring tiles
        for (int i = 0; i < n_offsets; i++) {
            //printf("(%d) propagating changes to %d, %d: %d\n", i, yStart + OFFSETS[i*2], xStart + OFFSETS[i*2+1], changes[i]);
            if (changes[i]) {
                li_push(data->updates, &(ivec2_t){xStart + OFFSETS[i*2+1], yStart + OFFSETS[i*2]});
            }
        }
        //printf("\n");
    }
}