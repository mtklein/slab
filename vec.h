#pragma once

#define   vec(N,T) T __attribute__((vector_size(N * sizeof(T))))
#define splat(V,x) (((V){0} + 1) * (x))
#define  cast(V,v) __builtin_convertvector(v,V)
