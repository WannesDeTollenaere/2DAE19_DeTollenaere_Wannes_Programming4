#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class GameObject;

	class Transform final : public Component
	{
	public:
		Transform(GameObject* owner) : Component(owner) {}

		const glm::vec3& GetLocalPosition() const { return m_localPosition; }
		void SetLocalPosition(float x, float y, float z = 0.0f);
		void SetLocalPosition(const glm::vec3& position);

		const glm::vec3& GetWorldPosition();

		float GetScale() const { return m_localScale; }
		void SetScale(float scale) { m_localScale = scale; SetPositionDirty(); }

		float GetWorldScale();

		void SetPositionDirty();

		void RenderGUI() override;

	private:

		glm::vec3 m_localPosition{};
		glm::vec3 m_worldPosition{};
		float m_localScale{ 1.0f };
		float m_worldScale{ 1.0f };
		bool m_positionIsDirty{ true };

		void UpdateWorldPosition();
	};
}
