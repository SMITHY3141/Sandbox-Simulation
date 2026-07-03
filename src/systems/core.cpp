

#include <stdio.h>

#include <EnTT/entt.hpp>
#include "systems/core.hpp"
#include "components/components.hpp"
#include "inputs.hpp"

#define MOVE_SPEED 0.1

void debug_print(double dt) {
    printf("fps: %lf\n", 1/dt);
}


void camera_controls(entt::registry *registry, InputState *state, double dt) {

    int mod = 1 + 10 * state->keys[sf::Keyboard::LShift];
    
    int forward = state->keys[sf::Keyboard::W] - state->keys[sf::Keyboard::S];
    int strafe = state->keys[sf::Keyboard::D] - state->keys[sf::Keyboard::A];

    double zoom = mod * dt * (state->keys[sf::Keyboard::Down] - state->keys[sf::Keyboard::Up]);
    if (zoom < -0.9) {
        zoom = -0.9;
    }

    if (!forward && !strafe && !zoom) {
        return;
    }



   	auto view = registry->view<Camera>();
	for (auto [entity, camera] : view.each()) {
        if (forward || strafe) {
            camera.slaved = false;

        }
        double scl = mod * camera.view.getSize().x * MOVE_SPEED * dt;
        camera.view.move(sf::Vector2f(strafe * scl, forward * scl));

        if (zoom) {
            camera.view.zoom(zoom + 1);

        }
	}
}

bool handle_pause(entt::registry *registry, InputState *state) {
    auto &context = registry->ctx().get<Context>();

    if (state->keys_click[sf::Keyboard::Space]) {
        context.paused = !context.paused;

    }
    context.step = false;
    if (state->keys_click[sf::Keyboard::Period]) {
        context.step = true;
    }
    return context.paused && !context.step;

}

void slave_camera(entt::registry *registry) {
    float x;
    float y;
    bool found = false;
	for (auto [entity, pos, sel] : registry->view<Position, Selectable>().each()) {
        if (!sel.selected) {
            continue;
        }
        x = pos.x;
        y = pos.y;
        found = true;
        break;

    }

    if (!found) {
        return;
    }

    auto view = registry->view<Camera>();
	for (auto [entity, camera] : view.each()) {
        if (!camera.slaved) {
            continue;
        }
        camera.view.setCenter({x, y});


    }
}


