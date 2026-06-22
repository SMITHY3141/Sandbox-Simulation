

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

#include "components/components.hpp"
#include "systems/rendering.hpp"

void set_view(sf::RenderWindow *window, entt::registry *registry) {
	auto view = registry->view<Camera>();
	for (auto [entity, camera] : view.each()) {
        window->setView(camera.view);

	}
}

void background(sf::RenderWindow *window) {
    sf::RectangleShape rect;
    sf::Vector2f rectanglePosition(-100, 0);
    rect.setPosition(rectanglePosition);
    rect.setSize(sf::Vector2f(50, 50));

    window->draw(rect);


    //sf::VertexArray grid(sf::Lines); //could also be Points, LineStrip, Triangles, TriangleStrip, TriangleFan

    /*/ Vertical lines
    for (float x = 0; x <= 600; x += 3.0)
    {
        grid.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)));
        grid.append(sf::Vertex(sf::Vector2f(x, 600), sf::Color(200, 200, 200)));
    }

    // Horizontal lines
    for (float y = 0; y <= 600; y += 3.0)
    {
        grid.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)));
        grid.append(sf::Vertex(sf::Vector2f(600, y), sf::Color(200, 200, 200)));
    }*/

    //window->draw(grid);


}

