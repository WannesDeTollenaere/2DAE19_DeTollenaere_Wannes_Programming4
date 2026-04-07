#pragma once
#include "imgui.h"

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void FixedUpdate() {}
		virtual void Update() {}
		virtual void Render() const {}
		virtual void RenderGUI() { ImGui::Checkbox("Is active", &m_IsActive); }
		void Destroy() { m_markedForDeletion = true; }
		bool IsMarkedForDeletion() const { return m_markedForDeletion; }
		void SetActive(bool active) { m_IsActive = active; }
		bool IsActive() const { return m_IsActive; }
	protected:
		explicit Component(GameObject* owner) : m_owner(owner), m_IsActive(true) {}

		GameObject* GetOwner() const { return m_owner; }

	private:
		GameObject* m_owner;
		bool m_markedForDeletion{ false };
		bool m_IsActive{ true };
	};
}