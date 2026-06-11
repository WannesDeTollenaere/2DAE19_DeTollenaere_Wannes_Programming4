#include "SceneManager.h"
#include "Scene.h"
#include <imgui.h>
#include <iostream>
#include "SceneLoader.h"
#include "GameTime.h"
#include "InputManager.h"

void dae::SceneManager::FixedUpdate()
{
    int firstActive = static_cast<int>(m_sceneStack.size()) - 1;
    for (int i = static_cast<int>(m_sceneStack.size()) - 1; i >= 0; --i)
    {
        firstActive = i;
        if (m_sceneStack[i].freezeBelow)
            break;
    }

    for (int i = firstActive; i < static_cast<int>(m_sceneStack.size()); ++i)
        m_sceneStack[i].scene->FixedUpdate();
}

void dae::SceneManager::Update()
{
    int firstActive = static_cast<int>(m_sceneStack.size()) - 1;
    for (int i = static_cast<int>(m_sceneStack.size()) - 1; i >= 0; --i)
    {
        firstActive = i;
        if (m_sceneStack[i].freezeBelow)
            break;
    }

    for (int i = firstActive; i < static_cast<int>(m_sceneStack.size()); ++i)
        m_sceneStack[i].scene->Update();
}

void dae::SceneManager::Render()
{
    for (auto& entry : m_sceneStack)
        entry.scene->Render();
}

void dae::SceneManager::RenderGUI()
{
    ImGui::Begin("Hierarchy");
    for (size_t i = 0; i < m_sceneStack.size(); ++i)
    {
        ImGui::PushID(m_sceneStack[i].scene);
        std::string sceneName = "Scene " + std::to_string(i);

        if (ImGui::TreeNodeEx(sceneName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            m_sceneStack[i].scene->RenderHierarchy(&m_pSelectedGameObject);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::End();

    ImGui::Begin("Inspector");
    if (m_pSelectedGameObject != nullptr)
        m_pSelectedGameObject->RenderGUI();
    else
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Select a GameObject to view its details.");
    ImGui::End();
}

dae::Scene& dae::SceneManager::CreateAndOwnScene()
{
    m_ownedScenes.emplace_back(new Scene());
    return *m_ownedScenes.back();
}

dae::Scene& dae::SceneManager::CreateScene()
{
    auto& scene = CreateAndOwnScene();
    m_sceneStack.push_back({ &scene, false });
    return scene;
}

void dae::SceneManager::Clear()
{
    m_sceneStack.clear();
    m_ownedScenes.clear();
    m_pSelectedGameObject = nullptr;
}

void dae::SceneManager::SetActiveScene(const std::string& sceneName)
{
    m_pendingTransition = { TransitionType::Replace, sceneName, false };
    m_hasPendingTransition = true;
}

void dae::SceneManager::PushScene(const std::string& sceneName, bool freezeBelow)
{
    m_pendingTransition = { TransitionType::Push, sceneName, freezeBelow };
    m_hasPendingTransition = true;
}

void dae::SceneManager::PopScene()
{
    m_pendingTransition = { TransitionType::Pop, {}, false };
    m_hasPendingTransition = true;
}

void dae::SceneManager::HandleLateSceneTransition()
{
    if (!m_hasPendingTransition)
        return;

    m_hasPendingTransition = false;
    const auto& t = m_pendingTransition;

    if (t.type == TransitionType::Replace)
    {
        GameTime::GetInstance().ClearAllTimers();
        InputManager::GetInstance().Clear();
        Clear();

        auto& scene = CreateAndOwnScene();
        dae::SceneLoader::LoadScene(scene, t.sceneName);
        m_sceneStack.push_back({ &scene, false });
    }
    else if (t.type == TransitionType::Push)
    {
        auto& scene = CreateAndOwnScene();
        dae::SceneLoader::LoadScene(scene, t.sceneName);
        m_sceneStack.push_back({ &scene, t.freezeBelow });
    }
    else if (t.type == TransitionType::Pop)
    {
        if (!m_sceneStack.empty())
        {
            Scene* top = m_sceneStack.back().scene;
            m_sceneStack.pop_back();

            // remove ownership
            m_ownedScenes.erase(
                std::remove_if(m_ownedScenes.begin(), m_ownedScenes.end(),
                    [top](const std::unique_ptr<Scene>& s) { return s.get() == top; }),
                m_ownedScenes.end());
        }
    }
}