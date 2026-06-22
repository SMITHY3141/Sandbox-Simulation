


#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

void init(entt::registry* registry);

void update(entt::registry* registry, double dt);

void render(entt::registry* registry, sf::RenderWindow* window);

#endif // RUNTIME_HPP
