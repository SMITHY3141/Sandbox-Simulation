


#include <stdlib.h>
#include <math.h>
#include "ode.h"

// just frees any heap memory to store the state (but not the struct itself)
void ode_free(System system) {
    free(system.state);

}

System ode_zeros(void (*f)(float*, float, float*), int size) {
    System system;
    system.t = 0.f;
    system.size = size;
    system.state = malloc(size * sizeof(float));
    system.f = f;

    return system;
}

void ode_integrate_euler(System *system, float dt) {
    float *state = system->state;

    float step[system->size];
    system->f(state, system->t, step);
    
    for (int i = 0; i < system->size; i++) {
        state[i] += step[i] * dt;

    }
    system->t += dt;

}

void ode_example(float *state, float time, float *result) {
    (void) time;
    float y = state[0];
    float v = state[1];

    result[0] = v; // y' =
    result[1] = -0.4 * v - 2 * sin(y); // y'' =

}


    




