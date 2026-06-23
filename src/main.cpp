

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include "runtime.hpp"
#include "inputs.hpp"

#define WINDOW_NAME "SANDBOX WINDOW"

#define START_WIDTH 800
#define START_HEIGHT 800
#define START_MAXIMISED 1


int main(int argc, char** argv) {
    
    sf::RenderWindow window(sf::VideoMode(START_WIDTH, START_HEIGHT), WINDOW_NAME);
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
        window.clear(sf::Color(252, 242, 255));

        render(&registry, &window, &font);

        window.display();

    }
    

    return 0;
}
