

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include <ODE/ode.h>

#include "runtime.hpp"
#include "components/components.hpp"
#include "systems/core.hpp"
#include "systems/ui.hpp"
#include "systems/model.hpp"
#include "systems/viewer.hpp"
#include "colours.hpp"


#define TIME_STEP 0.0
#define DEFAULT_SIZE 5.0
#define FONT_PATH "assets/fonts/Pixeltype.ttf"


void process_events(sf::RenderWindow *window, InputState *state) {
    sf::Event event;

    if (!state->keys[sf::Keyboard::LShift])
        memset(state->mouse_click, 0, sizeof(state->mouse_click));

    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();

        if (event.type == sf::Event::KeyPressed)
            state->keys[event.key.code] = true;

        if (event.type == sf::Event::KeyReleased)
            state->keys[event.key.code] = false;

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
            state->mousewrld = window->mapPixelToCoords(pixelPos);

        }

        if (event.type == sf::Event::MouseButtonPressed) {
            state->mouse_click[event.mouseButton.button] = true;
            state->mouse[event.mouseButton.button] = true;
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            state->mouse[event.mouseButton.button] = false;
            state->mouse_click[event.mouseButton.button] = false;

        }
    }
    if (state->keys[sf::Keyboard::Escape]) {
        window->close();
    }

    // TODO need one for mouse buttons too
}

void init(entt::registry *registry, sf::Font *font) {
    if (!font->loadFromFile(FONT_PATH)) {
        exit(1); // Could not load font path
    }

	auto camera = registry->create();
	registry->emplace<Camera>(camera,sf::View(sf::Vector2f(0.0, 0.0), sf::Vector2f(DEFAULT_SIZE, -DEFAULT_SIZE)));

}

void update(entt::registry *registry, InputState *state, double dt) {
    float sim_dt = dt;
	if (TIME_STEP)
		sim_dt = TIME_STEP;

    //debug_print(dt);

    camera_controls(registry, state, dt);


    spawn_particle(registry, state, ode_forcedspring);

    //TODO add ability to pause
    step_particles(registry, sim_dt);



}

void render(entt::registry *registry, sf::RenderWindow *window, sf::Font *font) {
    window->clear(COLOUR_CLEAR);
    set_view(window, registry);
   
    Bounds bounds = get_bounds(window);
    background(window, font, &bounds);

    draw_phasespace(window, &bounds, ode_forcedspring);
    draw_particles(window, registry, &bounds);




}
