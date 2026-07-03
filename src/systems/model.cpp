

#include <ODE/ode.h>
#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#include "inputs.hpp"
#include "systems/model.hpp"
#include "components/components.hpp"
#include <VECTORS/vectors.hpp>

#define GRAVITY 9.81




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

// one thing to note is that lift is the force normal to the incoming wind (it is not the force normal to the wing).
// so lift and drag are orthogonal to each other, and therefore the resultant force produced by a wing is the force due to drag plus this normal lift force.
// the actual mechanism behind the total force boils down to pressure differences across the wing, and newtonian deflections of incoming wind.

// air density kg/m^3
float find_air_density(float altitude) {
    return 1.22f;

}

Acceleration missile_acceleration(const Position &pos, const Velocity &vel, const Attitude &att, const Mass &mass, float motor_force) {
    float speed = vel.length();
    float rho = find_air_density(0.f);

    Vector forward = Vector::fromPolar(1, att.pitch);
    Vector normal = {-forward.y, forward.x}; // 90 degree counter clockwise

    Vector lift = {-vel.y, vel.x};

    Acceleration acc = {0.f, 0.f};
    acc += forward * motor_force;

    // 1/2 * p * v^2 * S * C_d, we assume C_d has a form similar to a sine curve, S is reference area it is constant
    // C_d is what captures most of the behaviour here

    // we assume they're both flat planes here so the area projects easily
    // found a figure online saying drag coeff is around 1.1 for a cylinder shaped object with large length to width
    float drag_norm = 0.5f * rho * std::abs(normal.dot(vel)) * 3.6f * 0.09f; // dot product includes a speed term implicitly, so vel^2
    float drag_cone = 0.5f * rho * std::abs(forward.dot(vel)) * M_PI * 0.09f * 0.09f * 0.2f; // area * 0.5 * density * coeff * speed^2, other speed is added later
    acc -= (drag_norm * 1.1f + drag_cone) * vel; // the speed^2 comes into play here when we scale vel0

    if (speed) {
        acc += drag_norm * std::abs(forward.dot(vel)) * lift / speed; // max at 45 degrees, C_l 
    }

    acc /= mass.point;
    acc.y -= GRAVITY;

    return acc;
    
}

float missile_torque(const Velocity &vel, const Attitude &att, const Mass &mass, const Fins &fins) {
    //Vector forward = Vector::fromPolar(1, att.pitch);
    Vector normal = {-vel.y, vel.x}; // 90 degree counter clockwise

    Vector fin = Vector::fromPolar(1, fins.position + att.pitch);

    // in the direction normal to fin
    float force = normal.dot(fin)  * vel.length() * 0.1f; // why the 0.1f?

    // now the force normal to the body
    float torque =  fins.offset * force * std::cos(2 * fins.position);


    return (torque - 40.f * att.rate) / mass.inertia; // damping
}

// TODO decrease and shift mass with motor burn;
float missile_burn(RocketMotor &motor, float dt) {
    if (!motor.active || motor.duration < 0) {
        return 0.f;
    }

    motor.duration -= dt;
    return motor.force;

}


void step_missiles(entt::registry *registry, float dt) {
    auto view = registry->view<Position, Velocity, Acceleration, Attitude, RocketMotor, Mass, Fins>();
	for (auto [entity, pos, vel, acc, att, motor, mass, fins] : view.each()) {

        float motor_force = missile_burn(motor, dt);
        acc = missile_acceleration(pos, vel, att, mass, motor_force);
        
        float torque = missile_torque(vel, att, mass, fins); 

        vel += acc * dt;
        pos += vel * dt;

        att.rate += torque * dt; 
        att.pitch += att.rate * dt;
        att.pitch = std::fmod(att.pitch, (float) (2 * M_PI));

        //printf("%f\n", vel.length());

    }
}
void update_trails(entt::registry *registry, float dt) {
    auto view = registry->view<Position, Trail>();
	for (auto [entity, pos, trail] : view.each()) {
        if (trail.delta_current > 0) {
            trail.delta_current -= dt;
            continue;
        }
        trail.delta_current = trail.delta_reset;
        if (trail.index >= TRAIL_SIZE - 1) {
            continue;
        }
        trail.data[trail.index++] = pos;
        trail.end++;

    }
}
