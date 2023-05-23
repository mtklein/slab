#pragma once

#define vec(N,T) T __attribute__((vector_size(N * sizeof(T))))
#define splat(T,v) (((T){0} + 1) * (v))
#define cast(T,v) __builtin_convertvector(v,T)
