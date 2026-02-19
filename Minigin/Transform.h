#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class Transform final
	{
	public:
		Transform(GameObject* owner) : m_owner(owner) {}

		const glm::vec3& GetLocalPosition() const { return m_localPosition; }
		void SetLocalPosition(float x, float y, float z = 0.0f);
		void SetLocalPosition(const glm::vec3& position);

		const glm::vec3& GetWorldPosition();

		void SetPositionDirty();

	private:
		GameObject* m_owner;

		glm::vec3 m_localPosition{};
		glm::vec3 m_worldPosition{};
		bool m_positionIsDirty{ true };

		void UpdateWorldPosition();
	};
}
