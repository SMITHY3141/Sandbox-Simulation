

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

#include "components/components.hpp"
#include "systems/rendering.hpp"

#define COLOUR_GRID sf::Color(200, 200, 200)

void set_view(sf::RenderWindow *window, entt::registry *registry) {
	auto view = registry->view<Camera>();
	for (auto [entity, camera] : view.each()) {
        window->setView(camera.view);

	}
}

float get_spacing(float ideal) {
    float power = std::pow(10.0f, std::floor(std::log10(ideal)));
    float normalised = ideal / power;

    if (normalised < 2.0f) {
        return power;

    }
    if (normalised < 5.0f) {
        return 2.0f * power;

    }

    return 5.0f * power;

}

void draw_grid(sf::RenderWindow *window, sf::View *view, float xspacing, float yspacing) {
    sf::Vector2f centre = view->getCenter();
    sf::Vector2f size = view->getSize();
    
    float left = centre.x - size.x * 0.5f;
    float right = centre.x + size.x * 0.5f;
    float top = centre.y - size.y * 0.5f;
    float bottom = centre.y + size.y * 0.5f;


    int xstart = std::floor(left / xspacing);
    int xend = std::ceil(right / xspacing);

    int ystart = std::ceil(bottom / yspacing);
    int yend = std::floor(top / yspacing);

    sf::VertexArray grid(sf::Lines); //could also be Points, LineStrip, Triangles, TriangleStrip, TriangleFan
    // Vertical lines
    for (int i = xstart; i <= xend; i++) {
        float x = i * xspacing;

        grid.append(sf::Vertex(sf::Vector2f(x, bottom), COLOUR_GRID));
        grid.append(sf::Vertex(sf::Vector2f(x, top), COLOUR_GRID));
    }
    // Horizontal lines
    for (int i = ystart; i <= yend; i++) {
        float y = i * yspacing;
        grid.append(sf::Vertex(sf::Vector2f(left, y), COLOUR_GRID));
        grid.append(sf::Vertex(sf::Vector2f(right, y), COLOUR_GRID));
    }

    window->draw(grid);
}
/*sf::Vector2i world_to_screen(sf::RenderWindow *window, sf::View *view, float x, float y) {
        sf::Vector2f topleft = view->getCenter() - view->getSize() * 0.5f;

        int wx = (x - topleft.x) * (window->getSize().x / view->getSize().x);
        int wy = (y - topleft.y) * (window->getSize().y / view->getSize().y);
        return {wx, wy};
}*/
void draw_gridtext(sf::RenderWindow *window, sf::View *view, sf::Font *font, float xspacing, float yspacing) {
    sf::Vector2f centre = view->getCenter();
    sf::Vector2f size = view->getSize();
    
    float left = centre.x - size.x * 0.5f;
    float right = centre.x + size.x * 0.5f;
    float top = centre.y - size.y * 0.5f;
    float bottom = centre.y + size.y * 0.5f;


    int xstart = std::floor(left / xspacing);
    int xend = std::ceil(right / xspacing);

    int ystart = std::ceil(bottom / yspacing);
    int yend = std::floor(top / yspacing);

    float xaxis = std::clamp(0.0f, left, right);
    float yaxis = std::clamp(0.0f, bottom, top);

    // x axis text
    for (int i = xstart; i <= xend; i++) {
        float x = i * xspacing;
        sf::Vector2i pixel = window->mapCoordsToPixel({x, yaxis}, *view);
        // sf::Vector2i pixel = world_to_screen(window, view, x, yaxis);

        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        text.setPosition((float) pixel.x + 4.f, (float) pixel.y + 2.f);

        char buf[32];
        std::snprintf(buf, sizeof(buf), "%g", x);
        text.setString(buf);

        window->draw(text);

    }

    // y axis text
    for (int i = ystart; i <= yend; i++) {
        float y = i * yspacing;
        sf::Vector2i pixel = window->mapCoordsToPixel({xaxis, y}, *view);
        // sf::Vector2i pixel = world_to_screen(window, view, xaxis, y);

        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        text.setPosition((float) pixel.x + 4.f, (float) pixel.y + 2.f);

        char buf[32];
        std::snprintf(buf, sizeof(buf), "%g", y);
        text.setString(buf);

        window->draw(text);

    }
}

void background(sf::RenderWindow *window, entt::registry *registry, sf::Font *font) {
    sf::View camera; // Need to find the viewable area
    auto view = registry->view<Camera>();
    for (auto [entity, cam] : view.each()) {
        camera = cam.view;
        break;

    }

    float xspacing = get_spacing(camera.getSize().x / 10.0f); // ideally 10 lines
    float yspacing = get_spacing(-camera.getSize().y / 10.0f);

    draw_grid(window, &camera, xspacing, yspacing);
    

    // weird bug, if we create a window with 600 x 800 pixels getDefaultView()
    // will still thinks it's this size even after resizing. 
    // mapCoordsToPixel() still works correctly even after stretching, but now
    // when we try to draw the text it gets transformed again (where SFML assumes
    // we're giving it coordinates in the original 600x800 pixels and transforms
    // it into the new pixel size).
    sf::View uiView(sf::FloatRect(0.f, 0.f, (float) window->getSize().x, (float) window->getSize().y));
    window->setView(uiView);
    //window->setView(window->getDefaultView());
    draw_gridtext(window, &camera, font, xspacing, yspacing);
    window->setView(camera);









}

