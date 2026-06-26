

#include <ODE/ode.h>
#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#include "inputs.hpp"
#include "systems/model.hpp"


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
