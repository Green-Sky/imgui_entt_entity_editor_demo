#pragma once

#include <entt/entt.hpp>

#include <string>
#include <imgui_entt_entity_editor.hpp>

namespace Components {

	struct Transform {
		float x = 0.f;
		float y = 0.f;
	};

	struct Velocity {
		float x = 0.f;
		float y = 0.f;
	};

} // Components


namespace MM {
	template <>
	void ComponentEditorWidget<Components::Transform>(entt::registry& reg, entt::registry::entity_type e);

	template <>
	void ComponentEditorWidget<Components::Velocity>(entt::registry& reg, entt::registry::entity_type e);
}

namespace Systems {

	// simple Velocity system with hardcoded border clamp
	void Velocity(entt::registry& reg, float delta);

} // Systems

