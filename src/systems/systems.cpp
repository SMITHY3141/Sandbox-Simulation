

#include <stdio.h>

#include <EnTT/entt.hpp>
#include "systems/systems.hpp"
#include "components/components.hpp"
#include "inputs.hpp"

#define MOVE_SPEED 0.1

void debug_print(entt::registry *registry) {
	auto view = registry->view<Camera>();
	for (auto [entity, camera] : view.each()) {
        printf("debug at entity %f\n", camera.view.getSize().x);

        
	}
}


void camera_controls(entt::registry *registry, InputState *state, double dt) {
   	auto view = registry->view<Camera>();
	for (auto [entity, camera] : view.each()) {
        double scl = camera.view.getSize().x * MOVE_SPEED * dt;
        int forward = state->keys[sf::Keyboard::W] - state->keys[sf::Keyboard::S];
        int strafe = state->keys[sf::Keyboard::D] - state->keys[sf::Keyboard::A];

        if (forward || strafe) {
            camera.view.move(sf::Vector2f(strafe * scl, forward * scl));
        }
        int zoom = state->keys[sf::Keyboard::Down] - state->keys[sf::Keyboard::Up];

        if (zoom) {
            double factor = zoom * dt;
            if (factor < -0.9) {
                factor = -0.9;
            }
            camera.view.zoom(factor + 1);

        }
	}
}

