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

static void *ctx = &ctx;
static bool key_eq(int k1, int k2, void *arg) {
    expect(arg == ctx);
    return k1 == k2;
}

static void basics(struct slab *s) {
    expect(slab_len(s) == 0);

    int jenny[] = {4,1,2, 8,6,7, 5,3,0,9};

    int M = 0;
    for (int i = 0; i < len(jenny); i++) {
        M += slab_insert(s, jenny[i], jenny+i);
    }
    expect(0 < M && M < len(jenny));

    for (int i = 0; i < M; i++) {
        void *val = NULL;
        expect( slab_lookup(s, jenny[i], &val, key_eq,ctx) && val == jenny+i);
    }
    for (int i = M; i < len(jenny); i++) {
        void *val = NULL;
        expect(!slab_lookup(s, jenny[i], &val, key_eq,ctx) && val == NULL);
    }
}

static void slab_holds_null(struct slab *s) {
    int key = 42;
    {
        void *val = s;
        expect(!slab_lookup(s,key,&val, key_eq,ctx) && val == s);
        expect( slab_insert(s,key,NULL) && slab_len(s) == 1);
        expect( slab_lookup(s,key,&val, key_eq,ctx) && val == NULL);
    }

    int M = 1;
    while (slab_insert(s,++key,NULL)) {
        M++;
    }

    {
        void *val = s;
        expect(slab_lookup(s,--key,&val, key_eq,ctx) && val == NULL);
    }
    expect(slab_len(s) == M);
}

static void slab_holds_self(struct slab *s) {
    void *val = NULL;
    expect(!slab_lookup(s,42,&val, key_eq,ctx) && val == NULL);

    int prev=0, len=0;
    while (slab_insert(s,42,s)) {
        prev = len;
        len = slab_len(s);
    }
    expect(len == prev+1);

    expect(slab_lookup(s,42,&val, key_eq,ctx) && val == s);
}

static void slab_holds_its_own_function_pointers(struct slab *s) {
    void *val = NULL;
    expect(!slab_lookup(s, 1, &val, key_eq,ctx) && val == NULL);
    expect(!slab_lookup(s, 2, &val, key_eq,ctx) && val == NULL);
    expect(!slab_lookup(s, 3, &val, key_eq,ctx) && val == NULL);
    expect(!slab_lookup(s, 4, &val, key_eq,ctx) && val == NULL);

    slab_insert(s, 1, (void*)slab_insert);
    slab_insert(s, 2, (void*)slab_lookup);
    slab_insert(s, 3, (void*)slab_len);
    slab_insert(s, 4, (void*)slab_alloc);

    expect(slab_lookup(s, 1, &val, key_eq,ctx) && val == (void*)slab_insert);
    expect(slab_lookup(s, 2, &val, key_eq,ctx) && val == (void*)slab_lookup);
    expect(slab_lookup(s, 3, &val, key_eq,ctx) && val == (void*)slab_len);
    expect(slab_lookup(s, 4, &val, key_eq,ctx) && val == (void*)slab_alloc);
}

int main(void) {
    test(basics);
    test(slab_holds_null);
    test(slab_holds_self);
    test(slab_holds_its_own_function_pointers);
    return 0;
}
