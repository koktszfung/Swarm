#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "swarm.h"
#include "hashgrid.h"

typedef struct {
    float *x1, *y1; 
    float *x2, *y2;
    float *r;
    uint64_t **cell_keys; 
} Wall;
