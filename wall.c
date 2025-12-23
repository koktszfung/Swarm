#include "wall.h"

Wall* wall_create(float x1, float y1, float x2, float y2) {
    Wall* wall = malloc(sizeof(*wall));
    if (!wall) {
        fprintf(stderr, "Failed to allocate memory for Wall");
        return NULL;
    }

    return wall;
}
