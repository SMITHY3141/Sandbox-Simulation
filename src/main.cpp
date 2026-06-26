

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include "runtime.hpp"
#include "inputs.hpp"

#define WINDOW_NAME "SANDBOX WINDOW"

#define START_WIDTH 800
#define START_HEIGHT 800
#define START_MAXIMISED 1
#define ALIASING 4


int main(int argc, char** argv) {
    (void)argc;
    (void)argv; // silence warnings
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = ALIASING;

    sf::RenderWindow window(sf::VideoMode(START_WIDTH, START_HEIGHT), WINDOW_NAME, sf::Style::Default, settings);
    //window.setVerticalSyncEnabled(true);


    // Initial simulation conditions
    // each simulation is considered an entity inside the registry
    sf::Font font;
    entt::registry registry; 
    init(&registry, &font);


    InputState inputs{};
    sf::Clock clock; // runs immediately

    while (window.isOpen()) {

        // Event input handling
        process_events(&window, &inputs);

        float dt = clock.restart().asSeconds();
        // Actual simulation logic
        update(&registry, &inputs, dt);


        // Rendering
        render(&registry, &window, &font);

        window.display();

    }

    // clean up entity stuff
    

    return 0;
}
