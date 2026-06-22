

#include "runtime.hpp"
#include <EnTT/entt.hpp>

#define WINDOW_NAME "GRAPHICS WINDOW"

#define START_WIDTH 800
#define START_HEIGHT 600
#define START_MAXIMISED 1
#define START_VSYNC 0


int main(int argc, char** argv) {
    entt::registry registry;

    init(&registry);


    while (1) {
        update(&registry, 0.0);
    }
    

}
