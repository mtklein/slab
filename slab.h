#pragma once
#include <stdbool.h>
#include <stdint.h>

struct slab *slab_alloc(void);

int slab_len(struct slab const*);

bool slab_insert(struct slab      *, intptr_t key, void  *val);
bool slab_lookup(struct slab const*, intptr_t key, void **val,
                 bool(*eq)(intptr_t k1, intptr_t k2, void *ctx), void *ctx);

// fn() should return true if slab_walk() should stop early, and slab_walk() returns true if it did.
bool slab_walk(struct slab const*, bool (*fn)(intptr_t key, void *val, void *ctx), void *ctx);
