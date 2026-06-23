



#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "inputs.hpp"
#include <EnTT/entt.hpp>

void debug_print(double dt);

void camera_controls(entt::registry *registry, InputState *state, double dt);


#endif // SYSTEMS_HPP


