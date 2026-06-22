

#include <stdio.h>

#include <EnTT/entt.hpp>
#include "systems/systems.hpp"
#include "components/components.hpp"

void debug_print(entt::registry* registry) {
	auto view = registry->view<Position, Camera>();
	for (auto [entity, pos, acc] : view.each()) {
        printf("debug at entity %d\n", entity);
        
	}
}

