#pragma once

struct slab *slab_alloc(void);

int slab_len(struct slab const*);

_Bool slab_insert(struct slab      *, void *key, void  *val);
_Bool slab_lookup(struct slab const*, void *key, void **val,
                  _Bool(*eq)(void *k1, void *k2, void *ctx), void *ctx);
