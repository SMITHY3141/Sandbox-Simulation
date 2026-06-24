
#include <ODE/ode.h>
#include <SFML/Graphics.hpp>

#include "systems/viewer.hpp"
#include "systems/ui.hpp"
#include "colours.hpp"
#include <cmath>

#define SEGS 6


// Chatgpt wrote this
void draw_line(sf::Vector2f a, sf::Vector2f b, float thickness, sf::Color color, sf::VertexArray *lines) {
    sf::VertexArray quad(sf::Quads, 4);

    sf::Vector2f dir = b - a;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0.f)
        return;

    dir /= length;

    sf::Vector2f normal(-dir.y, dir.x);

    sf::Vector2f offset = normal * (thickness * 0.5f);

    quad[0].position = a + offset;
    quad[1].position = b + offset;
    quad[2].position = b - offset;
    quad[3].position = a - offset;

    for (int i = 0; i < 4; ++i)
        quad[i].color = color;

    for (int i = 0; i < 4; ++i)
        lines->append(quad[i]);
}


// assumes a 2D ode
void draw_phasespace(sf::RenderWindow *window, Bounds *bounds, ode_function f) {
    float xspacing = bounds->xspacing / 2;
    float yspacing = bounds->yspacing / 2;
    int xstart = std::floor(bounds->left / xspacing);
    int xend = std::ceil(bounds->right / xspacing);

    int ystart = std::ceil(bounds->bottom / yspacing);
    int yend = std::floor(bounds->top / yspacing);

    sf::VertexArray lines(sf::Quads);   
    
    sf::View view = window->getView(); // need to restore it later
    sf::View uiView(sf::FloatRect(0.f, 0.f, (float) window->getSize().x, (float) window->getSize().y));
    window->setView(uiView);

    for (int i = xstart; i <= xend; i++) {
        float x = i * xspacing;
        for (int j = ystart; j <= yend; j++) {
            float y = j * yspacing;
            sf::Vector2i pixel = window->mapCoordsToPixel({x, y}, view);

            float result[2];
            float state[] = {x, y};
            f(state, 0, result);

            sf::Vector2f v(result[0], result[1]);
            float len = std::sqrt(v.x * v.x + v.y * v.y);
            sf::Vector2f direction;
            if (len) {
                direction = sf::Vector2f(14*v.x / len, 14*v.y / len);
            } else {
                direction = sf::Vector2f(0.0f, 0.0f);
            }

            draw_line(sf::Vector2f(pixel.x, pixel.y), sf::Vector2f(pixel.x + direction.x, pixel.y - direction.y), 3, COLOUR_ARROW, &lines);
        }
    }
    
    window->draw(lines);
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
    sf::View uiView(sf::FloatRect(0.f, 0.f, (float) window->getSize().x, (float) window->getSize().y));
    window->setView(uiView);


    sf::VertexArray circles(sf::Triangles);
    for (auto [entity, system] : registry->view<System>().each()) {
        if (system.size != 2) {
            continue;

        }
        float x = system.state[0];
        float y = system.state[1];
        // TODO maybe add support for other sizes (like plot against time for 1D)

        if (x < bounds->left || x > bounds->right || y < bounds->bottom || y > bounds->top) {
            continue;
        }
        sf::Vector2i pixel = window->mapCoordsToPixel({x, y}, view);

        add_circle(&circles, sf::Vector2f(pixel), 9, COLOUR_PARTICLE);

    }

    window->draw(circles);
    window->setView(view);
}
