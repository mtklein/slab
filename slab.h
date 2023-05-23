#pragma once
#include "vec.h"

#define K 8

struct State {
    vec(K,__fp16) r,g,b,a;
};

struct Effect {
    struct State (*fn)(struct State st, int end, void *ctx);
    void *ctx;
};

void run(struct Effect const effect[], int effects, int n);
