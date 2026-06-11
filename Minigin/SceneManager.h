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

		void Clear();
		Scene* GetActiveScene() { return m_sceneStack.empty() ? nullptr : m_sceneStack.back().scene; }

		void SetActiveScene(const std::string& sceneName);
		void PushScene(const std::string& sceneName, bool freezeBelow = true);
		void PopScene();

		void HandleLateSceneTransition();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		struct StackEntry
		{
			Scene* scene{ nullptr };
			bool freezeBelow{ false };
		};

		enum class TransitionType { Replace, Push, Pop };

		struct PendingTransition
		{
			TransitionType type{ TransitionType::Replace };
			std::string sceneName{};
			bool freezeBelow{ false };
		};

		std::vector<std::unique_ptr<Scene>> m_ownedScenes{};
		std::vector<StackEntry> m_sceneStack{};
		GameObject* m_pSelectedGameObject{ nullptr };
		bool m_dockLayoutInitialized{ false };

		bool m_hasPendingTransition{ false };
		PendingTransition m_pendingTransition{};

		Scene& CreateAndOwnScene();
	};
}