#ifndef WFC_H_
#define WFC_H_

#include <stdbool.h>

typedef struct wfcdata {
  bool ***tilemap; 
  bool ***constraints; 
  int n_constraints; 
  int map_width;
  int map_height; 
} wfcdata_t;


void waveFunctionCollapse(wfcdata_t *data, int startY, int startX);

#endif // WFC_H_