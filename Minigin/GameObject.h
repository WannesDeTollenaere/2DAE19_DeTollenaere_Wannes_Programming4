#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "Transform.h"
#include "Component.h"

namespace dae
{

	class GameObject final
	{
	public:
		void FixedUpdate();
		void Update();
		void Render() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Components
		template <class T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
			T* componentPtr = component.get();
			m_components.emplace_back(std::move(component));
			return componentPtr;
		}

		template <class T>
		T* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				if (T* castedComponent = dynamic_cast<T*>(component.get()))
					return castedComponent;
			}
			return nullptr;
		}

		template <class T>
		void RemoveComponent()
		{
			std::erase_if(m_components, [](const std::unique_ptr<Component>& c) {
				return dynamic_cast<T*>(c.get()) != nullptr;
				});
		}

		Transform& GetTransform() { return m_transform; }
		const Transform& GetTransform() const { return m_transform; }

		void SetPosition(float x, float y) { m_transform.SetPosition(x, y); }

		// deletion
		bool IsMarkedForDeletion() const { return m_markedForDeletion; }
		void Destroy() { m_markedForDeletion = true; }

	private:
		Transform m_transform{};
		bool m_markedForDeletion{ false };
		std::vector<std::unique_ptr<Component>> m_components{};

		void DestroyComponentsMarkedForDeletion();
	};
}