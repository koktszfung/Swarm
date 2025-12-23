#pragma once
#include <stdio.h>
#include <math.h>
#include "deps/uthash.h"

typedef struct {
    uint64_t key;       // for hash table
    size_t   row;       // row of index table stored
    size_t   count;     // current number of indices in cell
    size_t   capacity;  // max number of indices in cell
    UT_hash_handle hh;
} HashCell;

typedef struct {
    HashCell *hashtable;
    
    HashCell *pool;
    size_t    pool_count;
    size_t    pool_capacity;

    size_t   *indextable;
    size_t    cell_capacity;
    float     cell_dim;
} HashGrid;

static inline uint64_t cell_key(HashGrid* hashgrid, float x, float y) {
    uint64_t cx = floorf(x / hashgrid->cell_dim);
    uint32_t cy = floorf(y / hashgrid->cell_dim);
    return (cx << 32) | cy;
}

HashGrid* hashgrid_create(size_t pool_capacity, size_t cell_capacity, float cell_dim);
void hashgrid_add(HashGrid* hashgrid, float x, float y, size_t i);
void hashgrid_clear(HashGrid* hashgrid);
void hashgrid_destroy(HashGrid* hashgrid);
