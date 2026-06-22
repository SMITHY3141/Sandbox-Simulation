


#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <SFML/Graphics.hpp>

typedef struct {
    bool keys[sf::Keyboard::KeyCount];
    bool mouse[sf::Mouse::ButtonCount];
    sf::Vector2i mousePos;
} InputState;

#endif // INPUTS_HPP
