


#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include "inputs.hpp"

void process_events(sf::RenderWindow *window, InputState *state);

void init(entt::registry *registry, sf::Font *font);

void update(entt::registry *registry, InputState *state, double dt);

void render(entt::registry *registry, sf::RenderWindow *window, sf::Font *font);

#endif // RUNTIME_HPP
