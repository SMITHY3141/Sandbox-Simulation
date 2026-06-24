


#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <SFML/Graphics.hpp>

typedef struct {
    bool keys[sf::Keyboard::KeyCount];
    bool mouse[sf::Mouse::ButtonCount];
    bool mouse_click[sf::Mouse::ButtonCount];
    sf::Vector2f mousewrld;
} InputState;

#endif // INPUTS_HPP
