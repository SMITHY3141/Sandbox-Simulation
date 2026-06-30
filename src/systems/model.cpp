

#include <ODE/ode.h>
#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#include "inputs.hpp"
#include "systems/model.hpp"
#include "components/components.hpp"









void ode_forcedspring(float *state, float time, float *result) {
    (void) time;
    float y = state[0];
    float v = state[1];

    result[0] = v;
    result[1] = -0.4 * v - 2 * y - 9*y;


}

void spawn_particle(entt::registry *registry, InputState *state, void (*f)(float*, float, float*)) {
    if (!state->mouse_click[sf::Mouse::Button::Left]) {
        return;

    }

    System system = ode_zeros(f, 2);
    system.state[0] = state->mousewrld.x;
    system.state[1] = state->mousewrld.y;

	auto particle = registry->create();
	registry->emplace<System>(particle,system);

}




void step_particles(entt::registry *registry, float dt) {
    auto view = registry->view<System>();
	for (auto [entity, system] : view.each()) {
        ode_integrate_euler(&system, dt);

    }
}



// MISSILES
void step_missiles(entt::registry *registry, float dt) {
    auto view = registry->view<Position, Velocity, Acceleration, Attitude, RocketMotor, Mass>();
	for (auto [entity, pos, vel, acc, att, motor, mass] : view.each()) {
        Position pos0 = pos;
        Velocity vel0 = vel;
        Acceleration acc0 = acc;
        Attitude att0 = att;
        RocketMotor motor0 = motor;
        Mass mass0 = mass;


        // maybe decrease mass with motor burn;

        acc.x = 0;
        acc.y = 0;
        if (motor0.active && motor0.duration > 0) {
            motor.duration -= dt;

            acc.x = std::cos(att0.pitch) * motor0.force / mass0.point;
            acc.y = std::sin(att0.pitch) * motor0.force / mass0.point;
        
        }
        acc.x -= vel.x * 0.001;
        acc.y -= vel.y * 0.001;

        vel.x += acc0.x * dt;
        vel.y += acc0.y * dt;

        pos.x += vel0.x * dt;
        pos.y += vel0.y * dt;


        //ode_integrate_euler(&system, dt);

    }

}

