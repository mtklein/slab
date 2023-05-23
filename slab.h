#pragma once

struct State {
    __fp16 __attribute__((vector_size(16))) r,g,b,a;
};

struct Effect {
    struct State (*fn)(struct State st, int end, void *ctx);
    void *ctx;
};

void run(struct Effect const effect[], int effects, int n);
