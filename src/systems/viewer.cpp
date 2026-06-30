
#include <ODE/ode.h>
#include <SFML/Graphics.hpp>

#include "systems/viewer.hpp"
#include "systems/ui.hpp"
#include "components/components.hpp"
#include "colours.hpp"
#include <cmath>

#define SEGS 6

// Chatgpt wrote this
void draw_arrow(sf::Vector2f loc, sf::Vector2f dir, float thickness, sf::Color colour, sf::VertexArray *triangles) {
    sf::VertexArray triangle(sf::Triangles, 3);

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0.f)
        return;

    dir /= length;

    sf::Vector2f normal(-dir.y, dir.x);

    triangle[0].position = loc;
    triangle[1].position = loc - (5.f * dir + normal) * thickness; // magic number to stretch
    triangle[2].position = loc - (5.f * dir - normal) * thickness;

    for (int i = 0; i < 3; i++) {
        triangle[i].color = colour;
        triangles->append(triangle[i]);
    
    }
}


// assumes a 2D ode
void draw_phasespace(sf::RenderWindow *window, Bounds *bounds, ode_function f) {
    float xspacing = bounds->xspacing / 2;
    float yspacing = bounds->yspacing / 2;
    int xstart = std::floor(bounds->left / xspacing);
    int xend = std::ceil(bounds->right / xspacing);

    int ystart = std::ceil(bounds->bottom / yspacing);
    int yend = std::floor(bounds->top / yspacing);

    sf::VertexArray triangles(sf::Triangles);   
    
    sf::View view = window->getView(); // need to restore it later
    sf::View uiView(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
    window->setView(uiView);

    for (int i = xstart; i <= xend; i++) {
        float x = i * xspacing;
        for (int j = ystart; j <= yend; j++) {
            float y = j * yspacing;
            sf::Vector2i pixel = window->mapCoordsToPixel({x, y}, view);

            float result[2];
            float state[] = {x, y};
            f(state, 0, result);

            sf::Vector2f v(result[0], -result[1]); // have to invert because screen space
            draw_arrow(sf::Vector2f(pixel.x, pixel.y), v, 3, COLOUR_ARROW, &triangles);
        }
    }
    
    window->draw(triangles);
    window->setView(view);
}

void add_circle(sf::VertexArray *circles, sf::Vector2f centre, float radius, sf::Color colour) {
    for (int i = 0; i < SEGS; ++i) {
        float a0 = i * 2.f * M_PI / SEGS;
        float a1 = (i + 1) * 2.f * M_PI / SEGS;

        circles->append({centre, colour});

        circles->append({centre + sf::Vector2f(std::cos(a0), std::sin(a0)) * radius, colour});
        circles->append({centre + sf::Vector2f(std::cos(a1), std::sin(a1)) * radius, colour});
	
    }
}

void draw_particles(sf::RenderWindow *window, entt::registry *registry, Bounds *bounds) {
    sf::View view = window->getView(); // need to restore it later
    sf::View uiView(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
    window->setView(uiView);


    sf::VertexArray circles(sf::Triangles);
    for (auto [entity, transform]: registry->view<Transform>().each()) {
        float x = transform.position.x;
        float y = transform.position.y;
        // TODO maybe add support for other sizes (like plot against time for 1D)

        if (x < bounds->left || x > bounds->right || y < bounds->bottom || y > bounds->top) {
            continue;
        }
        sf::Vector2i pixel = window->mapCoordsToPixel(transform.position, view);

        add_circle(&circles, sf::Vector2f(pixel), 9, COLOUR_PARTICLE);

    }

    window->draw(circles);
    window->setView(view);
}
