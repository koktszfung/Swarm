#include "hashgrid.h"

HashGrid* hashgrid_create(size_t pool_capacity, size_t cell_capacity, float cell_dim) {
    HashGrid *hashgrid = malloc(sizeof(*hashgrid));
    if (!hashgrid) {
        fprintf(stderr, "Failed to allocate memory for HashGrid");
        return NULL;
    }

    // ensure a default capacity
    if (pool_capacity == 0) pool_capacity = 64;  
    if (cell_capacity == 0) cell_capacity = 64;

    hashgrid->indextable = malloc(pool_capacity * cell_capacity * sizeof(*hashgrid->indextable));
    if (!hashgrid->indextable) {
        fprintf(stderr, "Failed to allocate memory for HashGrid->indextable");
        return NULL;
    }
    hashgrid->pool = malloc(pool_capacity * sizeof(*hashgrid->pool));  // preallocated cells
    if (!hashgrid->pool) {
        fprintf(stderr, "Failed to allocate memory for HashGrid->pool");
        return NULL;
    }
    hashgrid->hashtable  = NULL;  // initialize hash table to NULL
    
    hashgrid->pool_count    = 0;
    hashgrid->pool_capacity = pool_capacity;
    hashgrid->cell_capacity = cell_capacity;
    hashgrid->cell_dim = cell_dim;

    return hashgrid;
}

void hashgrid_add(HashGrid *hashgrid, float x, float y, size_t i) {
    uint64_t key = cell_key(hashgrid, x, y);

    HashCell *cell = NULL;
    HASH_FIND(hh, hashgrid->hashtable, &key, sizeof(key), cell);

    // cell not in hash
    if (!cell) {
        // pool overflow
        if (hashgrid->pool_count >= hashgrid->pool_capacity) {
            fprintf(stderr, "Pool overflow\n");
            return;
        }

        // initialize new cell from the pool
        cell = &hashgrid->pool[hashgrid->pool_count++];  // use newest row
        cell->row = hashgrid->pool_count - 1;
        cell->count = 0;
        cell->capacity = hashgrid->cell_capacity;
        cell->key = key;

        HASH_ADD(hh, hashgrid->hashtable, key, sizeof(key), cell);
    }

    size_t row = cell->row;
    
    // cell overflow 
    if (cell->count >= cell->capacity) {
        fprintf(stderr, "Cell %zu overflow\n", i);
        return;
    }
    
    // insert index i
    size_t col = cell->count++;  // use newest col
    hashgrid->indextable[row * cell->capacity + col] = i;
}

void hashgrid_clear(HashGrid *hashgrid) {
    HashCell *cell, *tmp;
    HASH_ITER(hh, hashgrid->hashtable, cell, tmp) {
        HASH_DELETE(hh, hashgrid->hashtable, cell);
    }
    hashgrid->pool_count = 0;
}

void hashgrid_destroy(HashGrid *hashgrid) {
    if (!hashgrid) return;

    HASH_CLEAR(hh, hashgrid->hashtable);
    free(hashgrid->pool);
    free(hashgrid->indextable);
    
    free(hashgrid);
}
