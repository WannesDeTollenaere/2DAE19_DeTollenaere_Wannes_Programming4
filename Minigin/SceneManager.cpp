#define IMGUI_DEFINE_MATH_OPERATORS
#include "SceneManager.h"
#include "Scene.h"
#include <imgui.h>
#include <imgui_internal.h>
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
    // Fullscreen, transparent, pass-through dockspace host window 
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags hostFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("EditorDockSpace", nullptr, hostFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("MainDockSpace");

    if (!m_dockLayoutInitialized)
    {
        m_dockLayoutInitialized = true;

        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->WorkSize);

        ImGuiID centerId = dockspaceId;
        ImGuiID rightId = ImGui::DockBuilderSplitNode(centerId, ImGuiDir_Right, 0.25f, nullptr, &centerId);

        ImGuiID timeId = ImGui::DockBuilderSplitNode(rightId, ImGuiDir_Down, 0.12f, nullptr, &rightId);
        ImGuiID inspectorId = ImGui::DockBuilderSplitNode(rightId, ImGuiDir_Down, 0.5f, nullptr, &rightId);

        ImGui::DockBuilderDockWindow("Hierarchy", rightId);
        ImGui::DockBuilderDockWindow("Inspector", inspectorId);
        ImGui::DockBuilderDockWindow("Time", timeId);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.45f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.55f, 0.25f, 0.25f, 1.0f));
    ImGui::Begin("Time");
    ImGui::PopStyleColor(2);
    float timeScale = GameTime::GetInstance().GetTimeScale();
    if (ImGui::SliderFloat("Time Scale", &timeScale, 0.0f, 3.0f))
        GameTime::GetInstance().SetTimeScale(timeScale);

    ImGui::SameLine();
    if (ImGui::Button(timeScale == 0.0f ? "Resume" : "Pause"))
        GameTime::GetInstance().SetTimeScale(timeScale == 0.0f ? 1.0f : 0.0f);
    ImGui::End();

    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.20f, 0.35f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.25f, 0.45f, 0.25f, 1.0f));
    ImGui::Begin("Hierarchy");
    ImGui::PopStyleColor(2);
    if (ImGui::Button("Add GameObject") && !m_sceneStack.empty())
    {
        m_pSelectedGameObject = m_sceneStack.back().scene->CreateGameObject("GameObject");
    }
    ImGui::Separator();

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

    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.20f, 0.20f, 0.45f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.25f, 0.25f, 0.55f, 1.0f));
    ImGui::Begin("Inspector");
    ImGui::PopStyleColor(2);
    if (m_pSelectedGameObject != nullptr)
    {
        m_pSelectedGameObject->RenderGUI();

        ImGui::Separator();

        static const auto componentTypes = SceneLoader::GetRegisteredComponentTypes();
        static int selectedTypeIndex = 0;

        if (!componentTypes.empty())
        {
            if (ImGui::BeginCombo("##AddComponentType", componentTypes[selectedTypeIndex].c_str()))
            {
                for (int i = 0; i < static_cast<int>(componentTypes.size()); ++i)
                {
                    bool isSelected = (i == selectedTypeIndex);
                    if (ImGui::Selectable(componentTypes[i].c_str(), isSelected))
                        selectedTypeIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            if (ImGui::Button("Add Component"))
            {
                SceneLoader::AddComponentByType(m_pSelectedGameObject, componentTypes[selectedTypeIndex]);
            }
        }
    }
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