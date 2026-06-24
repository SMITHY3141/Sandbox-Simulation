

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

#include "components/components.hpp"
#include "systems/ui.hpp"

#include "colours.hpp"

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

// What can our camera actually see in the world
// also gives us some info about the grid spacing
Bounds get_bounds(sf::RenderWindow *window) {
    const sf::View& view = window->getView();

    sf::Vector2f centre = view.getCenter();
    sf::Vector2f size = view.getSize();
    
    Bounds bounds;
    bounds.left = centre.x - size.x * 0.5f;
    bounds.right = centre.x + size.x * 0.5f;
    bounds.top = centre.y - size.y * 0.5f;
    bounds.bottom = centre.y + size.y * 0.5f;

    // Try to fit 10 lines across the screen, but also make them spaced nicely
    bounds.xspacing = get_spacing(view.getSize().x / 10.0f);
    bounds.yspacing = get_spacing(-view.getSize().y / 10.0f);

    return bounds;

}

void draw_grid(sf::RenderWindow *window, const Bounds *bounds) {
    int xstart = std::floor(bounds->left / bounds->xspacing);
    int xend = std::ceil(bounds->right / bounds->xspacing);

    int ystart = std::ceil(bounds->bottom / bounds->yspacing);
    int yend = std::floor(bounds->top / bounds->yspacing);

    sf::VertexArray grid(sf::Lines); //could also be Points, LineStrip, Triangles, TriangleStrip, TriangleFan

    // Vertical lines
    for (int i = xstart; i <= xend; i++) {
        float x = i * bounds->xspacing;

        grid.append(sf::Vertex(sf::Vector2f(x, bounds->bottom), COLOUR_GRID));
        grid.append(sf::Vertex(sf::Vector2f(x, bounds->top), COLOUR_GRID));
    }
    // Horizontal lines
    for (int i = ystart; i <= yend; i++) {
        float y = i * bounds->yspacing;
        grid.append(sf::Vertex(sf::Vector2f(bounds->left, y), COLOUR_GRID));
        grid.append(sf::Vertex(sf::Vector2f(bounds->right, y), COLOUR_GRID));
    }
 
    // TODO draw thick lines at the Axes
    window->draw(grid);
}

void draw_gridtext(sf::RenderWindow *window, const sf::View *view, sf::Font *font, const Bounds *bounds) {
    int xstart = std::floor(bounds->left / bounds->xspacing);
    int xend = std::ceil(bounds->right / bounds->xspacing);

    int ystart = std::ceil(bounds->bottom / bounds->yspacing);
    int yend = std::floor(bounds->top / bounds->yspacing);

    // TODO check size of text so it doesn't clip outside (below screen, to right)
    float xaxis = std::clamp(0.0f, bounds->left, bounds->right);
    float yaxis = std::clamp(0.0f, bounds->bottom, bounds->top);


    // x axis text
    for (int i = xstart; i <= xend; i++) {
        float x = i * bounds->xspacing;
        sf::Vector2i pixel = window->mapCoordsToPixel({x, yaxis}, *view);

        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        text.setPosition((float) pixel.x + 8.f, (float) pixel.y);

        char buf[32];
        std::snprintf(buf, sizeof(buf), "%g", x);
        text.setString(buf);

        window->draw(text);

    }

    // y axis text
    for (int i = ystart; i <= yend; i++) {
        float y = i * bounds->yspacing;
        sf::Vector2i pixel = window->mapCoordsToPixel({xaxis, y}, *view);

        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        text.setPosition((float) pixel.x + 8.f, (float) pixel.y);

        char buf[32];
        std::snprintf(buf, sizeof(buf), "%g", y);
        text.setString(buf);

        window->draw(text);

    }
}

void background(sf::RenderWindow *window, sf::Font *font, const Bounds *bounds) {
    sf::View view = window->getView();

    draw_grid(window, bounds);

    // weird bug, if we create a window with 600 x 800 pixels getDefaultView()
    // will still thinks it's this size even after resizing. 
    // mapCoordsToPixel() still works correctly even after stretching, but now
    // when we try to draw the text it gets transformed again (where SFML assumes
    // we're giving it coordinates in the original 600x800 pixels and transforms
    // it into the new pixel size).
    sf::View uiView(sf::FloatRect(0.f, 0.f, (float) window->getSize().x, (float) window->getSize().y));
    window->setView(uiView);

    draw_gridtext(window, &view, font, bounds);

    window->setView(view);


}

