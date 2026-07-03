

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include "runtime.hpp"
#include "inputs.hpp"
#include "systems/manager.hpp"

#define WINDOW_NAME "SANDBOX WINDOW"

#define START_WIDTH 2560
#define START_HEIGHT 1440
#define START_MAXIMISED 1
#define ALIASING 4


int main(int argc, char** argv) {
    (void)argc; // maybe what save to use
    (void)argv; // silence warnings
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = ALIASING;

    sf::RenderWindow window(sf::VideoMode(START_WIDTH, START_HEIGHT), WINDOW_NAME, sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);


    // TODO add bounds checking to the rendering (also stop sprite rendering when too small, or somehow have it a fixed size as in it can only get so small)
    // TODO if we go down the route of removing sprites if too small, add some dot that has fixed size and takes over the sprite

    // TODO add a sync camera component system, so the camera is synced with some other entity position. We can exit the sync mode just by moving, but it should start on by default
    // TODO later on add a click and drag feature so we're able to highlight a certain entity and keep it in view (or maybe we can highlight several entities and then keep all of them just inside bounds)

    // TODO implement flight dynamics for the missile
    // we should have a seperate file similar to the ODE library, the same style of ODE function, except our system packages the components all into an array that we pass into the ode function
    // TODO add an attitude flight control system

    // TODO draw the fins independently of the missile. (either add a new component which would be hard with draw order, or somehow add a draw child sprite method or something).
    // Have to be careful about draw order here though, like idealy we draw it missile by missile, so for one missile we draw the body and then the fins

    // TODO add procedural terrain too

    // TODO add a save system, it just saves all the components associated with each entity and their values
    // TODO there should also be someway to convert a save into some code segment that could be put in the model_init()

    // TODO add a trail component

    // TODO add ODE viewers back, for example we may want to see how the pitch angle behaves around the current operating conditions


    // Initial simulation conditions
    // each simulation is considered an entity inside the registry
    sf::Font font;
    TextureManager textures;
    
    entt::registry registry; 
    view_init(&registry, &window, &textures, &font);
    model_init(&registry);

    InputState inputs{};
    sf::Clock clock; // runs immediately

    while (window.isOpen()) {
        // Event input handling
        process_events(&window, &inputs);


        float dt = clock.restart().asSeconds();
        // Actual simulation logic
        update(&registry, &inputs, dt);


        // Rendering
        render(&registry, &window, &textures, &font);

        window.display();

    }

    // clean up entity stuff
    

    return 0;
}
