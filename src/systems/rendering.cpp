

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

#include "systems/rendering.hpp"

void background(sf::RenderWindow* window) {
    sf::RectangleShape rect;
    sf::Vector2f rectanglePosition(300, 300);
    rect.setPosition(rectanglePosition);
    rect.setSize(sf::Vector2f(50, 50));

    window->draw(rect);


}

