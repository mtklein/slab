#pragma once
#include <stdint.h>

struct slab *slab_alloc(void);

int slab_len(struct slab const*);

_Bool slab_insert(struct slab      *, intptr_t key, void  *val);
_Bool slab_lookup(struct slab const*, intptr_t key, void **val,
                  _Bool(*eq)(intptr_t k1, intptr_t k2, void *ctx), void *ctx);
