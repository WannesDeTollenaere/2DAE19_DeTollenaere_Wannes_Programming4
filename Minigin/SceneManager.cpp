#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

#include <imgui.h>

void dae::SceneManager::RenderGUI()
{
    ImGui::Begin("Hierarchy");

    for (size_t i = 0; i < m_scenes.size(); ++i)
    {
        ImGui::PushID(m_scenes[i].get());

        std::string sceneName = "Scene " + std::to_string(i);

        if (ImGui::TreeNode(sceneName.c_str()))
        {
            m_scenes[i]->RenderGUI();
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    ImGui::End();
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}
