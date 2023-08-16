#include "slab.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define M 8
struct slab {
    void *key[M];
    void *val[M];
};

static char const not_yet_full = 0;

struct slab *slab_alloc(void) {
    struct slab *s = calloc(1, sizeof *s);
    s->val[M-1] = (union { void const *cptr; void *ptr; }){&not_yet_full}.ptr;
    return s;
}

int slab_len(struct slab const *s) {
    return s->val[M-1] == &not_yet_full ? (int)(intptr_t)s->key[M-1]
                                        : M;
}

bool slab_insert(struct slab *s, void *key, void  *val) {
    int const len = slab_len(s);
    if (len < M) {
        s->key[M-1] = (void*)(intptr_t)(len+1);
        s->key[len] = key;
        s->val[len] = val;
        return true;
    }
    return false;
}

bool slab_lookup(struct slab const *s, void *key, void **val,
                 bool(*eq)(void*, void*, void*), void *ctx) {
    for (int i = 0; i < slab_len(s); i++) {
        if (eq(key, s->key[i], ctx)) {
            *val = s->val[i];
            return true;
        }
    }
    return false;
}
