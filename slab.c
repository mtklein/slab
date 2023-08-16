#include "slab.h"
#include <stdbool.h>
#include <stdlib.h>

#define M 8
struct slab {
    intptr_t key[M];
    void    *val[M];
};

static void *not_yet_full(void) {
    static char const unique_address = 0;
    return (union {void const *cptr; void *ptr;}){&unique_address}.ptr;
}

struct slab *slab_alloc(void) {
    struct slab *s = calloc(1, sizeof *s);
    s->val[M-1] = not_yet_full();
    return s;
}

int slab_len(struct slab const *s) {
    return s->val[M-1] == not_yet_full() ? (int)s->key[M-1]
                                         : M;
}

bool slab_insert(struct slab *s, intptr_t key, void *val) {
    int const len = slab_len(s);
    if (len < M) {
        s->key[M-1] = len+1;
        s->key[len] = key;
        s->val[len] = val;
        return true;
    }
    return false;
}

bool slab_lookup(struct slab const *s, intptr_t key, void **val,
                 bool(*eq)(intptr_t, intptr_t, void*), void *ctx) {
    for (int i = 0; i < slab_len(s); i++) {
        if (eq(key, s->key[i], ctx)) {
            *val = s->val[i];
            return true;
        }
    }
    return false;
}
