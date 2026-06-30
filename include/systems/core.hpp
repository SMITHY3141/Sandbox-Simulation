



#ifndef CORE_HPP
#define CORE_HPP

#include "inputs.hpp"
#include <EnTT/entt.hpp>

void debug_print(double dt);

void camera_controls(entt::registry *registry, InputState *state, double dt);

bool handle_pause(entt::registry *registry, InputState *state);

#endif // CORE_HPP


