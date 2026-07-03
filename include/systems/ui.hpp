

#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <EnTT/entt.hpp>
#include "systems/manager.hpp"


typedef struct {
    float left;
    float right;
    float top;
    float bottom;
    float xspacing;
    float yspacing;
} Bounds;


void set_view(sf::RenderWindow *window, entt::registry *registry);
Bounds get_bounds(sf::RenderWindow *window);
void background(sf::RenderWindow *window, sf::Font *font, const Bounds *bounds);

void draw_thick(sf::Vector2f a, sf::Vector2f b, float thickness, sf::Color colour, sf::VertexArray *triangles);
void render_sprites(sf::RenderWindow *window, entt::registry *registry, TextureManager *textures, const Bounds *bounds);
void update_transforms(entt::registry *registry);



#endif // UI_HPP
