

#include <EnTT/entt.hpp>
#include <SFML/Graphics.hpp>

#include "components/components.hpp"
#include "systems/ui.hpp"
#include "systems/manager.hpp"

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
 
    window->draw(grid);

}

void draw_thick(sf::Vector2f A, sf::Vector2f B, float thickness, sf::Color colour, sf::VertexArray *triangles) {
    sf::VertexArray triangle(sf::Triangles, 6);

    sf::Vector2f dir = A - B;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0.f)
        return;

    dir /= length;

    sf::Vector2f normal(-dir.y * thickness, dir.x * thickness);

    
    triangle[0].position = A - normal;
    triangle[1].position = A + normal;
    triangle[2].position = B + normal;

    triangle[3].position = B + normal;
    triangle[4].position = B - normal;
    triangle[5].position = A - normal;

    for (int i = 0; i < 6; i++) {
        triangle[i].color = colour;
        triangles->append(triangle[i]);
    
    }
}

void draw_gridtext(sf::RenderWindow *window, const sf::View *view, sf::Font *font, const Bounds *bounds) {
    int xstart = std::floor(bounds->left / bounds->xspacing);
    int xend = std::ceil(bounds->right / bounds->xspacing);

    int ystart = std::ceil(bounds->bottom / bounds->yspacing);
    int yend = std::floor(bounds->top / bounds->yspacing);

    // TODO check size of text so it doesn't clip outside (below screen, to right)
    float xaxis = std::clamp(0.0f, bounds->left, bounds->right);
    float yaxis = std::clamp(0.0f, bounds->bottom, bounds->top);

    sf::VertexArray triangles(sf::Triangles);
    draw_thick(sf::Vector2f(window->mapCoordsToPixel({bounds->left, yaxis}, *view)), sf::Vector2f(window->mapCoordsToPixel({bounds->right, yaxis}, *view)), 1, COLOUR_AXIS, &triangles);
    draw_thick(sf::Vector2f(window->mapCoordsToPixel({xaxis, bounds->top}, *view)), sf::Vector2f(window->mapCoordsToPixel({xaxis, bounds->bottom}, *view)), 1, COLOUR_AXIS, &triangles);
    window->draw(triangles);


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

void update_transforms(entt::registry *registry) {
	auto view = registry->view<Position, Attitude, Transform>();
	for (auto [entity, pos, angle, data] : view.each()) {
        data.position = sf::Vector2f(pos.x, pos.y);
        data.rotation = 180.f * angle.pitch / M_PI;
    }

}

void render_sprites(sf::RenderWindow *window, entt::registry *registry, TextureManager *textures, const Bounds *bounds) {
	auto view = registry->view<Transform, SpriteComponent>();
	for (auto [entity, transform, data] : view.each()) {
        sf::Sprite sprite(textures->get(data.texture));
        sf::Vector2u textureSize = sprite.getTexture()->getSize();
        sprite.setOrigin(textureSize.x * data.offset.x, textureSize.y * data.offset.y);

        sprite.setPosition(transform.position);
        sprite.setRotation(transform.rotation);
        sprite.setScale(transform.scale);

        // Todo add bounds check

        window->draw(sprite);

	}
}
