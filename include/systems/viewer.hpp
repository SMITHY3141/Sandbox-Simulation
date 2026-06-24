



#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <ODE/ode.h>
#include <SFML/Graphics.hpp>
#include "systems/ui.hpp"

void draw_phasespace(sf::RenderWindow *window, Bounds *bounds, ode_function f);

void draw_particles(sf::RenderWindow *window, entt::registry *registry, Bounds *bounds);

#endif // VIEWER_HPP
