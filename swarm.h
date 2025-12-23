#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    size_t count;
    size_t capacity;
    float *r;
    float *x, *y;
    float *vx, *vy;
    float *ax, *ay;
} Swarm;

Swarm* swarm_create(size_t initial_capacity);
void   swarm_reserve(Swarm* s, size_t extra_capacity);
void   swarm_add(Swarm* s, float r, float x, float y, float vx, float vy, float ax, float ay);
void   swarm_remove(Swarm* s, size_t i);
void   swarm_destroy(Swarm* s);

void   repel(Swarm* swarm, size_t a1, size_t a2, const float k, const float c, const float mu, const float cutoff);