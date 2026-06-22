

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

#include "runtime.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"
#include "systems/rendering.hpp"


#define TIME_STEP 0.01
#define DEFAULT_SIZE 200.0


void process_events(sf::RenderWindow *window, InputState *state) {
    sf::Event event;

    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();

        if (event.type == sf::Event::KeyPressed)
            state->keys[event.key.code] = true;

        if (event.type == sf::Event::KeyReleased)
            state->keys[event.key.code] = false;

        if (event.type == sf::Event::MouseMoved)
            state->mousePos = {event.mouseMove.x, event.mouseMove.y};
    }

    // TODO need one for mouse buttons too
}

void init(entt::registry *registry) {
	auto camera = registry->create();
	registry->emplace<Camera>(camera,sf::View(sf::Vector2f(0.0, 0.0), sf::Vector2f(DEFAULT_SIZE, -DEFAULT_SIZE)));

}

void update(entt::registry *registry, InputState *state, double dt) {
    float sim_dt = dt;
	if (TIME_STEP)
		sim_dt = TIME_STEP;

    //debug_print(registry);
    camera_controls(registry, state, dt);



}

void render(entt::registry *registry, sf::RenderWindow *window) {
    set_view(window, registry);
    background(window);



}
