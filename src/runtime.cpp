


#include "runtime.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

#define TIME_STEP 0.01


void init(entt::registry* registry) {
	auto camera = registry->create();
	registry->emplace<Position>(camera, 0.0, 0.0);
	registry->emplace<Camera>(camera, 0.0, 0.0);
}

void update(entt::registry* registry, double dt) {
    float sim_dt = dt;
	if (TIME_STEP)
		sim_dt = TIME_STEP;

    debug_print(registry);
}
