#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(const GameObject& object);
		void RemoveAll();

		void FixedUpdate();
		void Update();
		void Render() const;
		void RenderGUI();
		void RenderHierarchy(GameObject** selectedObject);

		GameObject* GetGameObjectByTag(const std::string& tag) const;
		std::vector<GameObject*> GetGameObjectsByTag(const std::string& tag) const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;
		explicit Scene() = default;

		void DestroyGameObjectsMarkedForDeletion();

		std::vector < std::unique_ptr<GameObject>> m_objects{};
	};

}
