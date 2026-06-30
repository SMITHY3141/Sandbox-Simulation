

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


#define TIME_STEP 0.0 // if we want to set a custom time step, can essentially be a slow mode or fast mode
#define DEFAULT_SIZE 5.0f // horizontal viewable area
#define FONT_PATH "assets/fonts/Pixeltype.ttf"
#define TEXTURE_PATH "assets/textures/"

#define SAVE_PATH "assets/saves/"


void process_events(sf::RenderWindow *window, InputState *state) {
    sf::Event event;

    if (!state->keys[sf::Keyboard::LShift]) {
        memset(state->mouse_click, 0, sizeof(state->mouse_click));
    }
    memset(state->keys_click, 0, sizeof(state->keys_click));


    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();

        if (event.type == sf::Event::KeyPressed) {
            state->keys[event.key.code] = true;
            state->keys_click[event.key.code] = true;
        }

        if (event.type == sf::Event::KeyReleased) {
            state->keys[event.key.code] = false;
            //state->keys_click[event.key.code] = false;

        }

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
            //state->mouse_click[event.mouseButton.button] = false;

        }
    }
    if (state->keys[sf::Keyboard::Escape]) {
        window->close();

    }

    // TODO need one for mouse buttons too
}

void load_textures(TextureManager *textures) {
    textures->load(TextureID::Missile, TEXTURE_PATH "aim.png");

}

void view_init(entt::registry *registry, sf::RenderWindow *window, TextureManager *textures, sf::Font *font) {
    if (!font->loadFromFile(FONT_PATH)) { // TODO add FontManager
        exit(1); // Could not load font path
    }

    load_textures(textures);

    sf::Vector2u size = window->getSize();
	auto camera = registry->create();
	registry->emplace<Camera>(camera,sf::View(sf::Vector2f(0.0, 0.0), sf::Vector2f(DEFAULT_SIZE, -(size.y * DEFAULT_SIZE / size.x))));

}

void model_init(entt::registry *registry) {
    registry->ctx().emplace<Context>(true); // paused

    auto missile = registry->create();
    registry->emplace<Position>(missile, 1.f, 0.f);
    registry->emplace<Velocity>(missile, 0.f, 0.f);
    registry->emplace<Acceleration>(missile, 0.f, 0.f);
    registry->emplace<Attitude>(missile, 0.75f, 0.f);
    registry->emplace<RocketMotor>(missile, 500.f, true, 5.f);
    registry->emplace<Mass>(missile, 100.f, 1.f);
    registry->emplace<Fins>(missile, 0.f, 0.f, 0.f);

    registry->emplace<Transform>(missile, sf::Vector2f(0.f, 0.f), 0.f, sf::Vector2f(0.0045f, 0.0045f));
    registry->emplace<SpriteComponent>(missile, TextureID::Missile);

}

void update(entt::registry *registry, InputState *state, double dt) {
    float sim_dt = dt;
	if (TIME_STEP)
		sim_dt = TIME_STEP;

    //debug_print(dt);
    camera_controls(registry, state, dt);

    bool paused = handle_pause(registry, state);

    if (paused) {
        return;
    
    }

    step_missiles(registry, sim_dt);


    //spawn_particle(registry, state, ode_forcedspring);



    //step_particles(registry, sim_dt);


}



void render(entt::registry *registry, sf::RenderWindow *window, TextureManager *textures, sf::Font *font) {
    window->clear(COLOUR_CLEAR);
    set_view(window, registry);
   
    Bounds bounds = get_bounds(window);
    background(window, font, &bounds);

    //draw_phasespace(window, &bounds, ode_forcedspring);
    //draw_particles(window, registry, &bounds);

    update_transforms(registry); // updates Transform (used visually), with Position and Attitude components

    render_sprites(window, registry, textures, &bounds);
    draw_particles(window, registry, &bounds);


}
