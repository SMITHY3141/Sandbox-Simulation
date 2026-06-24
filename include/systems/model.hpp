


#ifndef MODEL_HPP
#define MODEL_HPP

#include <EnTT/entt.hpp>
#include "inputs.hpp" 

void spawn_particle(entt::registry *registry, InputState *state, void (*f)(float*, float, float*));
void step_particles(entt::registry *registry, float dt);

#endif // MODEL_HPP
