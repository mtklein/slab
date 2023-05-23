#include "slab.h"
#include "vec.h"
#include <stdint.h>

#define expect(x) if (!(x)) __builtin_trap()
#define len(x) (int)( sizeof(x) / sizeof((x)[0]) )
#define K len((struct State){0}.r)

typedef vec(K,   __fp16) F16;
typedef vec(K, uint16_t) U16;
typedef vec(K, uint32_t) U32;


struct Color { __fp16 r,g,b,a; };

static struct State uniform_color(struct State st, int end, void *ctx) {
    (void)end;

    vec(4, __fp16) c;
    _Static_assert(sizeof c == sizeof(struct Color), "");
    __builtin_memcpy(&c, ctx, sizeof c);

    st.r = splat(F16, c[0]);
    st.g = splat(F16, c[1]);
    st.b = splat(F16, c[2]);
    st.a = splat(F16, c[3]);
    return st;
}
static void test_uniform_color(void) {
    struct Color red = { 1,0,0,1 };
    struct Effect effect[] = {
        {uniform_color, &red},
    };
    run(effect,len(effect), 42);
    // TODO: write this out somewhere, check
}


static struct State load_rgba_8888(struct State st, int end, void *ctx) {
    vec(K, uint32_t) rgba = {0};
    {
        uint32_t const *p = ctx;
        if (end & (K-1)) { __builtin_memcpy(&rgba, p + (end-1), sizeof rgba[0]); }
        else             { __builtin_memcpy(&rgba, p + (end-K), sizeof rgba   ); }
    }
    st.r = cast(F16, cast(U16, (rgba >>  0) & 0xff)) * (__fp16)(1/255.0f);
    st.g = cast(F16, cast(U16, (rgba >>  8) & 0xff)) * (__fp16)(1/255.0f);
    st.b = cast(F16, cast(U16, (rgba >> 16) & 0xff)) * (__fp16)(1/255.0f);
    st.a = cast(F16, cast(U16, (rgba >> 24) & 0xff)) * (__fp16)(1/255.0f);
    return st;
}
static void test_load_rgba_8888(void) {
    uint32_t rgba[] = {
        0xffaaccee, 0xffff0000, 0xff00ff00, 0xff0000ff,
        0xffaaccee, 0xffff0000, 0xff00ff00, 0xff0000ff,
        0xffaaccee,
    };
    struct Effect effect[] = {
        {load_rgba_8888, rgba},
    };
    run(effect,len(effect), len(rgba));
    // TODO: write this out somewhere, check
}

static struct State count_steps(struct State st, int end, void *ctx) {
    int *steps = ctx;
    *steps += (end & (K-1)) ? 1 : K;
    return st;
}
static void test_count_steps(void) {
    for (int i = 0; i < 42; i++) {
        int steps = 0;
        struct Effect effect[] = {
            {count_steps, &steps},
        };
        run(effect,len(effect), i);
        expect(steps == i);
    }
}

int main(void) {
    test_count_steps();
    test_uniform_color();
    test_load_rgba_8888();
    return 0;
}
