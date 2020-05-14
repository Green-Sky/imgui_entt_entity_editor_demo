#include "./velocity.hpp"

#include <algorithm>

namespace MM {
	template <>
	void ComponentEditorWidget<Components::Transform>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& t = reg.get<Components::Transform>(e);
		// the "##Transform" ensures that you can use the name "x" in multiple lables
		ImGui::DragFloat("x##Transform", &t.x, 0.1f);
		ImGui::DragFloat("y##Transform", &t.y, 0.1f);
	}

	template <>
	void ComponentEditorWidget<Components::Velocity>(entt::registry& reg, entt::registry::entity_type e)
	{
		auto& v = reg.get<Components::Velocity>(e);
		ImGui::DragFloat("x##Velocity", &v.x, 0.1f);
		ImGui::DragFloat("y##Velocity", &v.y, 0.1f);
	}
}

namespace Systems {

void Velocity(entt::registry& reg, float delta) {
	reg.view<Components::Transform, Components::Velocity>().each(
	[&](Components::Transform& trans, Components::Velocity& vel) {
		trans.x += vel.x * delta;
		trans.y += vel.y * delta;

		if (trans.x < 0.f || trans.x > 1280.f) {
			trans.x = std::clamp(trans.x, 0.f, 1280.f);
			vel.x *= -1.f;
		}

		if (trans.y < 0.f || trans.y > 720.f) {
			trans.y = std::clamp(trans.y, 0.f, 720.f);
			vel.y *= -1.f;
		}
	});
}

} // Systems

