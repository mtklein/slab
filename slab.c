#include "slab.h"

void run(struct Effect const effect[], int const effects, int n) {
    for (int end = 0; n;) {
        end += n < K ? 1 : K;
        n   -= n < K ? 1 : K;

        struct State st = {0};
        for (int i = 0; i < effects; i++) {
            st = effect[i].fn(st, end, effect[i].ctx);
        }
    }
}
