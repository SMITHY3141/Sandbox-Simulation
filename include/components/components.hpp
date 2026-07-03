


#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "systems/manager.hpp"
#include <VECTORS/vectors.hpp>

#define TRAIL_SIZE 300

// Model

struct Position : Vector {
};
struct Velocity : Vector {
};
struct Acceleration : Vector {
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
    float position;
    float velocity;
    float offset;
};


// Rendering

struct Camera {
    sf::View view;
    bool slaved; // is this looking at an entity?

};

struct Transform { // Meant for sprite position, seperate to the model
    sf::Vector2f position;
    float rotation;
    sf::Vector2f scale = {1.f, 1.f};

};

struct SpriteComponent {
    TextureID texture;
    sf::Vector2f offset = {0.f, 0.f};
    // sf::IntRect textureRect; // what part of the texture to use?

};

struct Selectable {
    bool selected; // is this focused?

};


struct Context {
    bool paused;
    bool step;

};

struct Trail {
    float delta_reset;

    float delta_current = 0;
    int end = 0;
    int index = 0;
    Position data[TRAIL_SIZE] = {};

};
#endif // COMPONENTS_HPP


