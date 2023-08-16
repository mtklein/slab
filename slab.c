#include "slab.h"
#include <stdbool.h>
#include <stdlib.h>

#define M 8
struct slab {
    int   key[M];
    void *val[M];
};

struct slab *slab_alloc(void) {
    struct slab *s = calloc(1, sizeof *s);
    s->val[M-1] = (void*)1;
    return s;
}

int slab_len(struct slab const *s) {
    return s->val[M-1] == (void*)1 ? (int)s->key[M-1]
                                   : M;
}

bool slab_insert(struct slab *s, int key, void *val) {
    int const len = slab_len(s);
    if (len < M) {
        s->key[M-1] = len+1;
        s->key[len] = key;
        s->val[len] = val;
        return true;
    }
    return false;
}

struct lookup_ctx {
    int    key;
    int    padding;
    void **val;
    bool (*eq)(int, int, void*);
    void  *ctx;
};

static bool lookup(int key, void *val, void *ctx) {
    struct lookup_ctx *lookup_ctx = ctx;
    if (lookup_ctx->eq(key, lookup_ctx->key, lookup_ctx->ctx)) {
        *lookup_ctx->val = val;
        return true;
    }
    return false;
}

bool slab_lookup(struct slab const *s, int key, void **val,
                 bool(*eq)(int, int, void*), void *ctx) {
    struct lookup_ctx lookup_ctx = {.key=key,.val=val,.eq=eq,.ctx=ctx};
    return slab_walk(s, lookup, &lookup_ctx);
}

bool slab_walk(struct slab const *s, bool (*fn)(int key, void *val, void *ctx), void *ctx) {
    for (int i = 0; i < slab_len(s); i++) {
        if (fn(s->key[i], s->val[i], ctx)) {
            return true;
        }
    }
    return false;
}
