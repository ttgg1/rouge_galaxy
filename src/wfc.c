#include "wfc.h"
#include <stdbool.h>

int *OFFSETS[] = {
    {-1, 0}, // NORTH
    {0, 1},  // EAST
    {1, 0},  // SOUTH
    {0, -1}, // WEST
};

int n_offsets = 4;



void wave_function_collapse(wfcdata_t *data, int start_y, int start_x) {

    if (start_y < 0 || start_y >= data->map_height || start_x < 0 || start_x >= data->map_width) {
        // return if starting tile is out-of-bounds
        return;
    }

    bool changes[n_offsets];
    for (int i = 0; i < n_offsets; i++) {
        changes[i] = false;
    }
    
    // iterate over adjacent neighbours (N,E,S,W) of starting tile

    int pos_y, pos_x;
    for (int direction = 0; direction < n_offsets; direction++) {
        pos_y = start_y + OFFSETS[direction][0];
        pos_x = start_x + OFFSETS[direction][1];

        // skip out-of-bounds
        if (pos_y < 0 || pos_y >= data->map_height || pos_x < 0 || pos_x >= data->map_width) {
            continue;
        }

        // determine possible tiles for neighbour 

        bool superpos[data->n_constraints];
        for (int i = 0; i < data->n_constraints; i++) {
            superpos[i] = false;
        }

        for (int idx_cons = 0; idx_cons < data->n_constraints; idx_cons++) {
            if (!data->tilemap[start_y][start_x][idx_cons]) {
                continue;
            }
            // if tile is in the starting tiles allowed tileset, add its constraintset to possible tiles for neighbour
            for (int idx = 0; idx < data->n_constraints; idx++) {
                if (data->constraints[idx_cons][direction][idx]) {
                    superpos[idx] = true;
                }
            }
        }

        // update allowed tiles for neighbour

        for (int i = 0; i < data->n_constraints; i++) {
            if (data->tilemap[pos_y][pos_x][i] && !superpos[i]) {
                data->tilemap[pos_y][pos_x][i] = false;
                
                // mark neighbour if tile is removed from neighbours tileset
                changes[direction] = true;
            }   
        }
    }

    // continue propagating changes to updated neighbouring tiles
    for (int i = 0; i < n_offsets; i++) {
        if (changes[i]) {
            wave_function_collapse(data, pos_y + OFFSETS[i][0], pos_x + OFFSETS[i][1]);
        }
    }
}
