

#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <SFML/Graphics.hpp>
#include <EnTT/entt.hpp>


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




#endif // RENDERING_HPP
