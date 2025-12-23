#pragma once
#include <stdlib.h>

static inline float randf(void) { return (float)rand() / (float)RAND_MAX; }
static inline float randf_max(float max) { return max * randf(); }
static inline float randf_range(float min, float max) { return min + (max - min) * randf(); }