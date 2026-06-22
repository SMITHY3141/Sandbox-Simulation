

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>
#include "runtime.hpp"

#define WINDOW_NAME "SANDBOX WINDOW"

#define START_WIDTH 800
#define START_HEIGHT 600
#define START_MAXIMISED 1
#define START_VSYNC 0


int main(int argc, char** argv) {
    
    sf::RenderWindow window(sf::VideoMode(START_WIDTH, START_HEIGHT), WINDOW_NAME);
    window.setFramerateLimit(60);


    // Initial simulation conditions
    // each simulation is considered an entity inside the registry
    entt::registry registry; 
    init(&registry);


    while (window.isOpen()) {

        // Event input handling
        sf::Event event;
        while (window.pollEvent(event)) // puts data into event var
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Actual simulation logic
        update(&registry, 0.0);


        // Rendering
        window.clear(sf::Color(252, 242, 255));

        render(&registry, &window);

        window.display();

    }
    

    return 0;
}
