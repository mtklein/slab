#pragma once

struct slab *slab_alloc(void);

int slab_len(struct slab const*);

_Bool slab_insert(struct slab      *, int key, void  *val);
_Bool slab_lookup(struct slab const*, int key, void **val,
                  _Bool(*eq)(int k1, int k2, void *ctx), void *ctx);

// fn() should return true if slab_walk() should stop early, and slab_walk() returns true if it did.
_Bool slab_walk(struct slab const*, _Bool (*fn)(int key, void *val, void *ctx), void *ctx);
