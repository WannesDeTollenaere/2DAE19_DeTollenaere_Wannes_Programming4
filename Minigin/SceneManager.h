#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();

		void FixedUpdate();
		void Update();
		void Render();
		void RenderGUI();
		void Clear() { m_scenes.clear(); }
		Scene* GetActiveScene() { return m_scenes.empty() ? nullptr : m_scenes.back().get(); }
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
		GameObject* m_pSelectedGameObject{ nullptr };
	};
}
