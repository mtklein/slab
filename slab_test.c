#include "expect.h"
#include "slab.h"
#include <stdbool.h>
#include <stdlib.h>

#define len(x) (int)(sizeof x / sizeof x[0])

static void test(void (*fn)(struct slab*)) {
    struct slab *s = slab_alloc();
    fn(s);
    free(s);
}

static _Bool ptr_eq(void *k1, void *k2, void *ctx) {
    (void)ctx;
    return k1 == k2;
}

static void basics(struct slab *s) {
    expect(slab_len(s) == 0);

    int jenny[] = {4,1,2, 8,6,7, 5,3,0,9};

    int M = 0;
    for (int i = 0; i < len(jenny); i++) {
        M += slab_insert(s, (void*)(intptr_t)jenny[i], jenny+i);
    }
    expect(0 < M && M < len(jenny));

    for (int i = 0; i < M; i++) {
        void *val = NULL;
        expect( slab_lookup(s, (void*)(intptr_t)jenny[i], &val, ptr_eq,NULL) && val == jenny+i);
    }
    for (int i = M; i < len(jenny); i++) {
        void *val = NULL;
        expect(!slab_lookup(s, (void*)(intptr_t)jenny[i], &val, ptr_eq,NULL) && val == NULL);
    }
}

static void slab_holds_self(struct slab *s) {
    void *val = NULL;
    expect(!slab_lookup(s,s,&val, ptr_eq,NULL) && val == NULL);

    int prev=0, len=0;
    while (slab_insert(s,s,s)) {
        prev = len;
        len = slab_len(s);
    }
    expect(len == prev+1);

    expect(slab_lookup(s,s,&val, ptr_eq,NULL) && val == s);
}

int main(void) {
    test(basics);
    test(slab_holds_self);
    return 0;
}
