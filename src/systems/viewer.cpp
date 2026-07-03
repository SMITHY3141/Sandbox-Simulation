
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

        add_circle(&circles, sf::Vector2f(pixel), 9.f, COLOUR_PARTICLE);

    }

    window->draw(circles);
    window->setView(view);
}


void draw_missile_debug(sf::RenderWindow *window, entt::registry *registry, Bounds *bounds) {
    sf::View view = window->getView(); // need to restore it later
    sf::View uiView(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
    window->setView(uiView);

    // TODO add bounds check

    sf::VertexArray triangles(sf::Triangles);
    for (auto [entity, transform, vel, att, fins]: registry->view<Transform, Velocity, Attitude, Fins>().each()) {
        sf::Vector2f pixel = sf::Vector2f(window->mapCoordsToPixel(transform.position, view));

        sf::Vector2f dir(std::cos(att.pitch), -std::sin(att.pitch));
        sf::Vector2f norm(-dir.y, dir.x);
        draw_thick(pixel, pixel + dir * 27.f, 3.f, COLOUR_ARROW, &triangles); // attitude indicator
        //draw_thick(pixel - dir * 10.f, pixel - dir * 10.f + norm * fins.position * 15.f, 2.f, COLOUR_ARROW, &triangles);

        // draw_arrow(sf::Vector2f(pos.x, pos.y) + dir * fins.offset, sf::Vector2f(std::cos(fins.position + att.pitch), std::sin(fins.position + att.pitch)), 0.1, COLOUR_ARROW, &triangles);
        
        float speed = vel.length();
        if (!speed) {
            continue;
        }

        sf::Vector2f velocity(vel.x, -vel.y);
        velocity = velocity / speed;
        draw_arrow(pixel + 27.f * velocity, velocity, 3.5f, COLOUR_PARTICLE, &triangles);
        
        //draw_arrow(sf::Vector2f(pos.x, pos.y), sf::Vector2f(vel.x, vel.y), 0.1, COLOUR_ARROW, &triangles);

    }


    window->draw(triangles);
    window->setView(view);
}

void draw_trails(sf::RenderWindow *window, entt::registry *registry, Bounds *bounds) {
    sf::View view = window->getView(); // need to restore it later
    sf::View uiView(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
    window->setView(uiView);

    // TODO add bounds check

    sf::VertexArray triangles(sf::Triangles);
    for (auto [entity, pos, trail]: registry->view<Position, Trail>().each()) {
        sf::Vector2f last;
        if (!trail.end) {
            continue;

        }
        for (int i = 0; i < trail.end; i++) {
            Position data = trail.data[i];
            sf::Vector2f position(data.x, data.y);
            sf::Vector2f pixel = sf::Vector2f(window->mapCoordsToPixel(position, view));
            if (!i) {
                last = pixel;
                continue;
            }

            draw_thick(pixel, last, 1.f, COLOUR_ARROW, &triangles); 
            last = pixel;

        }
        sf::Vector2f position(pos.x, pos.y);
        sf::Vector2f pixel = sf::Vector2f(window->mapCoordsToPixel(position, view));
        draw_thick(pixel, last, 1.f, COLOUR_ARROW, &triangles);


    }

    window->draw(triangles);
    window->setView(view);

}


