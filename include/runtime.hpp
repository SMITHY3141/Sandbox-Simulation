


#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include "systems/manager.hpp"
#include "inputs.hpp"

void process_events(sf::RenderWindow *window, InputState *state);

void load_textures(TextureManager *textures);

void view_init(entt::registry *registry, sf::RenderWindow *window, TextureManager *textures, sf::Font *font);

void model_init(entt::registry *registry);

void update(entt::registry *registry, InputState *state, double dt);

void render(entt::registry *registry, sf::RenderWindow *window, TextureManager *textures, sf::Font *font);

#endif // RUNTIME_HPP
