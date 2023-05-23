#include "slab.h"

void run(struct Effect const effect[], int effects, int n) {
    for (int i = 0; i < n/K*K; i += K) {
        struct State st = {0};
        for (struct Effect const *e = effect, *end = e+effects; e != end; e++) {
            st = e->fn(st, i + K, e->ctx);
        }
    }
    for (int i = n/K*K; i < n; i += 1) {
        struct State st = {0};
        for (struct Effect const *e = effect, *end = e+effects; e != end; e++) {
            st = e->fn(st, i + 1, e->ctx);
        }
    }
}
