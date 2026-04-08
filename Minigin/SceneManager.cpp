#include "SceneManager.h"
#include "Scene.h"
#include <imgui.h>
#include <iostream>
#include "SceneLoader.h"

void dae::SceneManager::FixedUpdate()
{
    for (auto& scene : m_scenes)
    {
        scene->FixedUpdate();
    }
}

void dae::SceneManager::Update()
{
    for (auto& scene : m_scenes)
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

void dae::SceneManager::RenderGUI()
{
    ImGui::Begin("Hierarchy");
    for (size_t i = 0; i < m_scenes.size(); ++i)
    {
        ImGui::PushID(m_scenes[i].get());
        std::string sceneName = "Scene " + std::to_string(i);

        if (ImGui::TreeNodeEx(sceneName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            m_scenes[i]->RenderHierarchy(&m_pSelectedGameObject);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::End();

    ImGui::Begin("Inspector");
    if (m_pSelectedGameObject != nullptr)
    {
        m_pSelectedGameObject->RenderGUI();
    }
    else
    {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Select a GameObject to view its details.");
    }
    ImGui::End();
}

dae::Scene& dae::SceneManager::CreateScene()
{
    m_scenes.emplace_back(new Scene());
    return *m_scenes.back();
}


void dae::SceneManager::SetActiveScene(const std::string& sceneName)
{
    m_SceneToLoad = sceneName;
    m_LoadSceneNextFrame = true;
}

void dae::SceneManager::HandleLateSceneTransition()
{
    if (m_LoadSceneNextFrame)
    {
        Clear();

        auto& scene = CreateScene();

        dae::SceneLoader::LoadScene(scene, m_SceneToLoad);

        m_LoadSceneNextFrame = false;
    }
}