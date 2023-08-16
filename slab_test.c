#include "expect.h"
#include "slab.h"
#include <stdbool.h>
#include <stdlib.h>

#define len(x) (int)(sizeof x / sizeof x[0])

static _Bool key_eq(void *k1, void *k2, void *ctx) {
    (void)ctx;
    return k1 == k2;
}

int main(void) {
    struct slab *s = slab_alloc();
    expect(slab_len(s) == 0);

    int jenny[] = {4,1,2, 8,6,7, 5,3,0,9};

    int n = 0;
    for (int i = 0; i < len(jenny); i++) {
        n += slab_insert(s, (void*)(intptr_t)jenny[i], jenny+i);
    }

    expect(0 < n && n < len(jenny));

    for (int i = 0; i < n; i++) {
        void *val = NULL;
        expect( slab_lookup(s, (void*)(intptr_t)jenny[i], &val, key_eq, NULL) && val == jenny+i);
    }
    for (int i = n; i < len(jenny); i++) {
        void *val = NULL;
        expect(!slab_lookup(s, (void*)(intptr_t)jenny[i], &val, key_eq, NULL) && val == NULL);
    }

    free(s);
    return 0;
}
