/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 **********************************************************************************************************************/

#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "laser.h"


typedef struct {
    Laser* data;
    size_t size;
    size_t capacity;
} LaserVec;


static inline void lasers_init(LaserVec* laserVec) {
    laserVec->data     = NULL;
    laserVec->size     = 0;
    laserVec->capacity = 0;
}


static inline void lasers_free(LaserVec* laserVec) {
    free(laserVec->data);
    laserVec->data     = NULL;
    laserVec->size     = 0;
    laserVec->capacity = 0;
}


static inline bool lasers_reserve(LaserVec* laserVec, size_t new_cap) {
    if (new_cap <= laserVec->capacity) return true;
    size_t cap = laserVec->capacity ? laserVec->capacity : 4;
    while (cap < new_cap) cap *= 2;
    Laser* p = (Laser*)realloc(laserVec->data, cap * sizeof(*p));
    if (!p) return false;
    laserVec->data = p;
    laserVec->capacity  = cap;
    return true;
}


static inline bool lasers_push(LaserVec* laserVec, Laser l) {
    if (laserVec->size == laserVec->capacity && !lasers_reserve(laserVec, laserVec->size + 1)) return false;
    laserVec->data[laserVec->size++] = l;
    return true;
}


static inline void lasers_remove_unordered(LaserVec* v, size_t i) {
    if (i >= v->size) return;
    v->data[i] = v->data[v->size - 1];
    v->size--;
}


static inline void lasers_compact_inactive(LaserVec* v) {
    size_t w = 0;
    for (size_t r = 0; r < v->size; ++r) {
        if (v->data[r].active) {
            if (w != r) v->data[w] = v->data[r];
            ++w;
        }
    }
    v->size = w;
}


