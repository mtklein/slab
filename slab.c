#include "slab.h"
#include "vec.h"

#define len(x) (int)( sizeof(x) / sizeof((x)[0]) )

void run(struct Effect const effect[], int const effects, int n) {
    for (int end = 0; n;) {
        struct State st = {0};
        int const K = len(st.r);

        end += n < K ? 1 : K;
        n   -= n < K ? 1 : K;

        for (int i = 0; i < effects; i++) {
            st = effect[i].fn(st, end, effect[i].ctx);
        }
    }
}
