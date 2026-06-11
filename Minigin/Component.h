#pragma once
#include "imgui.h"
#include <vector>
#include <string>
#include <variant>
#include <type_traits>
#include <glm/glm.hpp>

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
		void SetActive(bool active) { m_IsActive = active; }
		bool IsActive() const { return m_IsActive; }

		virtual void RenderGUI()
		{
			ImGui::Checkbox("Is active", &m_IsActive);

			for (auto& prop : m_exposedProperties)
			{
				std::visit([&](auto* ptr)
					{
						using T = std::decay_t<decltype(*ptr)>;
						if constexpr (std::is_same_v<T, int>)
							ImGui::DragInt(prop.name.c_str(), ptr);
						else if constexpr (std::is_same_v<T, unsigned int>)
							ImGui::DragInt(prop.name.c_str(), reinterpret_cast<int*>(ptr));
						else if constexpr (std::is_same_v<T, float>)
							ImGui::DragFloat(prop.name.c_str(), ptr, 0.1f);
						else if constexpr (std::is_same_v<T, double>)
						{
							float temp = static_cast<float>(*ptr);
							if (ImGui::DragFloat(prop.name.c_str(), &temp, 0.1f))
								*ptr = static_cast<double>(temp);
						}
						else if constexpr (std::is_same_v<T, bool>)
							ImGui::Checkbox(prop.name.c_str(), ptr);
						else if constexpr (std::is_same_v<T, glm::vec2>)
							ImGui::DragFloat2(prop.name.c_str(), &ptr->x, 0.1f);
						else if constexpr (std::is_same_v<T, glm::vec3>)
							ImGui::DragFloat3(prop.name.c_str(), &ptr->x, 0.1f);
					}, prop.value);
			}
		}
	protected:
		explicit Component(GameObject* owner) : m_owner(owner), m_IsActive(true) {}

		GameObject* GetOwner() const { return m_owner; }

		template <typename T>
		void ExposeProperty(const std::string& name, T* value)
		{
			static_assert(
				std::is_same_v<T, int> || std::is_same_v<T, unsigned int> ||
				std::is_same_v<T, float> || std::is_same_v<T, double> ||
				std::is_same_v<T, bool> ||
				std::is_same_v<T, glm::vec2> || std::is_same_v<T, glm::vec3>,
				"ExposeProperty only supports int, unsigned int, float, double, bool, glm::vec2 and glm::vec3 atm");

			m_exposedProperties.push_back({ name, value });
		}

	private:
		struct ExposedProperty
		{
			std::string name;
			std::variant<int*, unsigned int*, float*, double*, bool*, glm::vec2*, glm::vec3*> value;
		};

		GameObject* m_owner;
		bool m_markedForDeletion{ false };
		bool m_IsActive{ true };
		std::vector<ExposedProperty> m_exposedProperties;
	};
}

// expose a member using this macro for imgui
#define EXPOSE(member) ExposeProperty(#member, &member)
