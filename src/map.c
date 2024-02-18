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
    
    t->value = '.';
    m->t = t;
    m->height = 0;
    return m;
}

void m_destroy(map_t *m) {
    if (m == NULL) {
        return;
    }
    if (m->t != NULL) {
        qt_destroy(m->t);
    }
    free(m);
}


bool m_inBounds(map_t *m, int y, int x) {
    printf("inBounds: y: %d, x: %d, h: %d | %d %d %d %d\n", y, x, m->height, -(1 << (m->height - 1)) + 1, (1 << (m->height - 1)), -(1 << (m->height - 1)) + 1, (1 << (m->height - 1)));
    if (m->height == 0) {
        return y == 0 && x == 0;
    }
    
    return y >= -(1 << (m->height - 1)) + 1
        && y <= (1 << (m->height - 1))
        && x >= -(1 << (m->height - 1)) + 1
        && x <= (1 << (m->height - 1)); 
}

bool m_inBoundsNW(map_t *m, int y, int x) {
    return y >= -(1 << (m->height - 1)) + 1
        && y <= 0
        && x >= -(1 << (m->height - 1)) + 1
        && x <= 0; 
}

bool m_inBoundsNE(map_t *m, int y, int x) {
    return y >= -(1 << (m->height - 1)) + 1
        && y <= 0
        && x >= 1
        && x <= (1 << (m->height - 1)); 
}

bool m_inBoundsSW(map_t *m, int y, int x) {
    return y >= 1
        && y <= (1 << (m->height - 1))
        && x >= -(1 << (m->height - 1)) + 1
        && x <= 0; 
}

bool m_inBoundsSE(map_t *m, int y, int x) {
    return y >= 1
        && y <= (1 << (m->height - 1))
        && x >= 1
        && x <= (1 << (m->height - 1)); 
}

bool qt_isLeaf(quadTree_t *t) {
    return t->nw == NULL && t->ne == NULL && t->sw == NULL && t->se == NULL;
}

/*
    returns sub-tree which contains coordinates (x,y) as well as changing height and offset values
    passed used for translating local sub-tree coordinates to absolute values
*/
quadTree_t *m_getContainingTree(map_t *m, int y, int x, int *height, int *offY, int *offX) {

    // offsets for translating local tree coordinates
    int offsetY, offsetX, h;
    offsetY = *offY;
    offsetX = *offX;

    h = *height;

    quadTree_t *current = m->t;
    while (!qt_isLeaf(current) && h > 0) {
        
        if (m_inBoundsNW(m, y+offsetY, x+offsetX)) {

            offsetY += (1 << (h - 2));
            offsetX += (1 << (h - 2));

            current = current->nw;
            h--;
        }
        else if (m_inBoundsNE(m, y+offsetY, x+offsetX)) {

            offsetY += (1 << (h - 2));
            offsetX -= (1 << (h - 2));

            current = current->ne;
            h--;
        }
        else if (m_inBoundsSW(m, y+offsetY, x+offsetX)) {

            offsetY -= (1 << (h - 2));
            offsetX += (1 << (h - 2));

            current = current->sw;
            h--;
        }
        else if (m_inBoundsSE(m, y+offsetY, x+offsetX)) {

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


void m_setAt(map_t *m, int y, int x, char c) {

    // offsets for translating local tree coordinates
    int offsetY, offsetX, h;
    offsetY = 0;
    offsetX = 0;

    h = m->height;

    if (m_inBounds(m, y, x)) {
        // (x,y) in initialized map area
        quadTree_t *tree = m_getContainingTree(m, y, x, &h, &offsetY, &offsetX);

        if (h == 0) {
            // containing tree is leaf, base case
            tree->value = c;
            return;
        }

        // containing tree isnt leaf
        quadTree_t *current = tree; 
        while (h > 0) {

            // create sub-trees
            current->nw = qt_create();
            current->nw->value = current->value;
            current->ne = qt_create();
            current->ne->value = current->value;
            current->sw = qt_create();
            current->sw->value = current->value;
            current->se = qt_create();
            current->se->value = current->value;
            current->value = NULL;

            if (m_inBoundsNW(m, y+offsetY, x+offsetX)) {

                offsetY += (1 << (h - 2));
                offsetX += (1 << (h - 2));

                current = current->nw;
                h--;
            }
            else if (m_inBoundsNE(m, y+offsetY, x+offsetX)) {

                offsetY += (1 << (h - 2));
                offsetX -= (1 << (h - 2));

                current = current->ne;
                h--;
            }
            else if (m_inBoundsSW(m, y+offsetY, x+offsetX)) {

                offsetY -= (1 << (h - 2));
                offsetX += (1 << (h - 2));

                current = current->sw;
                h--;
            }
            else if (m_inBoundsSE(m, y+offsetY, x+offsetX)) {

                offsetY -= (1 << (h - 2));
                offsetX -= (1 << (h - 2));

                current = current->se;
                h--;
            }
        }
        current->value = c;
        return;
    }

    // (x,y) not in initialized area

    // expand tree
    quadTree_t *newTree = qt_create();
    char null = NULL;

    // NW
    newTree->nw = qt_create();
    newTree->nw->nw = qt_create();
    newTree->nw->ne = qt_create();
    newTree->nw->sw = qt_create();
    newTree->nw->se = m->t->nw;

    /*
    newTree->nw->nw->value = null;
    newTree->nw->ne->value = null;
    newTree->nw->sw->value = null;
    */

    // NE
    newTree->ne = qt_create();
    newTree->ne->nw = qt_create();
    newTree->ne->ne = qt_create();
    newTree->ne->sw = m->t->ne;
    newTree->ne->se = qt_create(); 

    /*
    newTree->ne->nw->value = null;
    newTree->ne->ne->value = null;
    newTree->ne->se->value = null;
    */

    // SW
    newTree->sw = qt_create();
    newTree->sw->nw = qt_create();
    newTree->sw->ne = m->t->sw;
    newTree->sw->sw = qt_create();
    newTree->sw->se = qt_create(); 

    /*
    newTree->sw->nw->value = null;
    newTree->sw->sw->value = null;
    newTree->sw->se->value = null;
    */

    // SE
    newTree->se = qt_create();
    newTree->se->nw = m->t->se;
    newTree->se->ne = qt_create();
    newTree->se->sw = qt_create();
    newTree->se->se = qt_create(); 

    /*
    newTree->se->ne->value = null;
    newTree->se->sw->value = null;
    newTree->se->se->value = null;
    */

    quadTree_t *temp = m->t;
    temp->nw = NULL;
    temp->ne = NULL;
    temp->sw = NULL;
    temp->se = NULL;
    m->t = newTree;
    qt_destroy(temp);
    m->height++;
    m_setAt(m, y, x, c);
}

char *m_getAt(map_t *m, int y, int x) {
    if (!m_inBounds(m, y, x)) {
        // not in initialized area
        return '.';
    }
    // offsets for translating local tree coordinates
    int offsetY, offsetX, h;
    offsetY = 0;
    offsetX = 0;

    h = m->height;
    quadTree_t *tree = m_getContainingTree(m, y, x, &h, &offsetY, &offsetX);
    return tree->value;
}
