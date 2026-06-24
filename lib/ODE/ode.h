



#ifndef ODE_H
#define ODE_H

typedef void (*ode_function)(float*, float, float*);

typedef struct {
    float* state;
    float t;
    int size;
    ode_function f; ///< float *state, float time, float *result
} System;

#ifdef __cplusplus
extern "C" {
#endif
void ode_free(System system);

System ode_zeros(void (*f)(float*, float, float*), int size);
void ode_integrate_euler(System *system, float dt);



void ode_example(float *state, float time, float *result);

#ifdef __cplusplus
}
#endif
#endif // ODE_H
