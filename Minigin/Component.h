#pragma once

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
		void Destroy() { m_markedForDeletion = true; }
		bool IsMarkedForDeletion() const { return m_markedForDeletion; }
	protected:
		explicit Component(GameObject* owner) : m_owner(owner) {}

		GameObject* GetOwner() const { return m_owner; }

	private:
		GameObject* m_owner;
		bool m_markedForDeletion{ false };
	};
}