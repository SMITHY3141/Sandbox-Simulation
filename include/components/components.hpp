


#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "systems/manager.hpp"

struct Camera {
    sf::View view;

};

struct Position {
    float x;
    float y;

};

struct Velocity {
    float x;
    float y;
};

struct Acceleration {
    float x;
    float y;

};

struct Attitude {
    float pitch;
    float rate;

};

struct RocketMotor {
    float force;
    bool active;
    float duration;
};

struct Mass {
    float point;
    float inertia;
};

struct Fins {
    float offset;
    float position;
    float velocity;
};

struct Transform { // Meant for sprite position, seperate to the model
    sf::Vector2f position;
    float rotation;
    sf::Vector2f scale = {1.f, 1.f};

};

struct SpriteComponent {
    TextureID texture;
    // sf::IntRect textureRect; // what part of the texture to use?

};


struct Context {
    bool paused;

};



#endif // COMPONENTS_HPP


