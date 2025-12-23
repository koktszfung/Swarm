#include "swarm.h"

Swarm* swarm_create(size_t initial_capacity) {
    Swarm *swarm = malloc(sizeof(*swarm));
    if (swarm == NULL) {
        fprintf(stderr, "Failed to allocate memory for Swarm");
        return NULL;
    }

    if (initial_capacity == 0) initial_capacity = 64;  // Ensure a default capacity

    swarm->r  = malloc(initial_capacity * sizeof(*swarm->r));
    swarm->x  = malloc(initial_capacity * sizeof(*swarm->x));
    swarm->y  = malloc(initial_capacity * sizeof(*swarm->y));
    swarm->vx = malloc(initial_capacity * sizeof(*swarm->vx));
    swarm->vy = malloc(initial_capacity * sizeof(*swarm->vy));
    swarm->ax = malloc(initial_capacity * sizeof(*swarm->ax));
    swarm->ay = malloc(initial_capacity * sizeof(*swarm->ay));
    if (!swarm->r || !swarm->x || !swarm->y || !swarm->vx || !swarm->vy || !swarm->ax || !swarm->ay) {
        fprintf(stderr, "Failed to allocate memory for Swarm property");
        return NULL;
    }
    swarm->count    = 0;
    swarm->capacity = initial_capacity;

    return swarm;
}

void swarm_reserve(Swarm *swarm, size_t extra_capacity) {
    if (swarm->count + extra_capacity <= swarm->capacity) return;

    size_t new_capacity = swarm->capacity + extra_capacity;
    if (new_capacity < swarm->capacity * 2) new_capacity = swarm->capacity * 2;  // exponential growth

    swarm->r  = realloc(swarm->r,  new_capacity * sizeof(*swarm->r));
    swarm->x  = realloc(swarm->x,  new_capacity * sizeof(*swarm->x));
    swarm->y  = realloc(swarm->y,  new_capacity * sizeof(*swarm->y));
    swarm->vx = realloc(swarm->vx, new_capacity * sizeof(*swarm->vx));
    swarm->vy = realloc(swarm->vy, new_capacity * sizeof(*swarm->vy));
    swarm->ax = realloc(swarm->ax, new_capacity * sizeof(*swarm->ax));
    swarm->ay = realloc(swarm->ay, new_capacity * sizeof(*swarm->ay));

    swarm->capacity = new_capacity;
}

void swarm_add(Swarm *swarm, float r, float x, float y, float vx, float vy, float ax, float ay) {
    if (swarm->count == swarm->capacity)
        swarm_reserve(swarm, swarm->capacity);

    swarm->r [swarm->count] = r;
    swarm->x [swarm->count] = x;
    swarm->y [swarm->count] = y;
    swarm->vx[swarm->count] = vx;
    swarm->vy[swarm->count] = vy;
    swarm->ax[swarm->count] = ax;
    swarm->ay[swarm->count] = ay;
    
    swarm->count++;
}

void swarm_remove(Swarm *swarm, size_t i) {
    if (i >= swarm->count) return;
    swarm->count--;
    if (i != swarm->count) {
        swarm->r [i] = swarm->r [swarm->count];
        swarm->x [i] = swarm->x [swarm->count];
        swarm->y [i] = swarm->y [swarm->count];
        swarm->vx[i] = swarm->vx[swarm->count];
        swarm->vy[i] = swarm->vy[swarm->count];
        swarm->ax[i] = swarm->ax[swarm->count];
        swarm->ay[i] = swarm->ay[swarm->count];
    }
}

void swarm_destroy(Swarm *swarm) {
    if (!swarm) return;

    free(swarm->r);
    free(swarm->x);
    free(swarm->y);
    free(swarm->vx);
    free(swarm->vy);
    free(swarm->ax);
    free(swarm->ay);
    
    free(swarm);
}

void repel(Swarm* swarm, size_t a1, size_t a2, const float k, const float c, const float mu, const float cutoff) {
    if (a1 <= a2) return;
    float dx = swarm->x[a2] - swarm->x[a1];
    float dy = swarm->y[a2] - swarm->y[a1];
    float d2 = dx * dx + dy * dy;
    if (d2 < 1e-12f || d2 > cutoff * cutoff) return;
    float d = sqrtf(d2);
    const float r0 = swarm->r[a1] + swarm->r[a2];  // preferred distance
    float delta = r0 - d; // overlap
    if (delta <= 0) return;
    // Normal direction
    float nx = dx / d;
    float ny = dy / d;
    // Relative velocity
    float dvx = swarm->vx[a1] - swarm->vx[a2];
    float dvy = swarm->vy[a1] - swarm->vy[a2];
    float v_n = dvx * nx + dvy * ny;
    // Normal force
    float f_normal = k * delta + c * v_n;
    // Tangential velocity
    float v_tx = dvx - v_n * nx;
    float v_ty = dvy - v_n * ny;
    float v_t_mag = sqrtf(v_tx * v_tx + v_ty * v_ty);
    // Tangential force
    float f_friction_x = 0.0f;
    float f_friction_y = 0.0f;
    if (v_t_mag > 1e-6f) {
        float f_friction_mag = -mu * fabsf(f_normal);
        f_friction_x = f_friction_mag * (v_tx / v_t_mag);
        f_friction_y = f_friction_mag * (v_ty / v_t_mag);
    }
    // Total force
    float f_total_x = f_normal * nx + f_friction_x;
    float f_total_y = f_normal * ny + f_friction_y;
    swarm->ax[a1] -= f_total_x;
    swarm->ay[a1] -= f_total_y;
    swarm->ax[a2] += f_total_x;
    swarm->ay[a2] += f_total_y;
}