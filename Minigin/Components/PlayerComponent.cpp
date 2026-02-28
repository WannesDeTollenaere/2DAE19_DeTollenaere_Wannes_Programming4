#include "PlayerComponent.h"
#include "GameObject.h"
#include "AnimatorComponent.h"
#include <imgui.h>
// TODO: needs to be moved to the actual game later


dae::PlayerComponent::PlayerComponent(GameObject* owner) : Component(owner)
{
    //struct AnimationClip
    //{
    //    int startRow;
    //    int startCol;
    //    int frameCount;
    //    float frameTime;
    //    bool isLooping;
    //};
}

void dae::PlayerComponent::InitializeAnimations()
{
    m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
    if (!m_pAnimator) return; 

    m_pAnimator->AddAnimation("Idle", { 1, 0, 1, 0.1f, true });
    m_pAnimator->AddAnimation("WalkDown", { 0, 0, 3, 0.1f, true });
    m_pAnimator->AddAnimation("WalkUp", { 0, 6, 3, 0.1f, true });
    m_pAnimator->AddAnimation("WalkLeft", { 0, 3, 3, 0.1f, true });
    m_pAnimator->AddAnimation("WalkRight", { 0, 9, 3, 0.1f, true });
    m_pAnimator->AddAnimation("Die", { 1, 3, 6, 0.2f, false });

    //SetState("WalkDown");
    SetState("Idle");
    //SetState("WalkUp");
    //SetState("WalkLeft");
    //SetState("WalkRight");
    //SetState("Die");
    m_animationsInitialized = true;
}

void dae::PlayerComponent::Update()
{
    if (!m_animationsInitialized)
    {
        InitializeAnimations();
    }
}

void dae::PlayerComponent::SetState(const std::string& stateName)
{
    if (m_currentState != stateName)
    {
        m_currentState = stateName;
        if (m_pAnimator)
        {
            m_pAnimator->PlayAnimation(m_currentState);
        }
    }
}
void dae::PlayerComponent::RenderGUI()
{

    ImGui::Begin("Player anim Test");

    ImGui::Text("Current State: %s", m_currentState.c_str());
    ImGui::Separator();

    if (ImGui::Button("Idle"))       SetState("Idle");
    if (ImGui::Button("Walk Down"))  SetState("WalkDown");
    if (ImGui::Button("Walk Up"))    SetState("WalkUp");
    if (ImGui::Button("Walk Left"))  SetState("WalkLeft");
    if (ImGui::Button("Walk Right")) SetState("WalkRight");

    ImGui::Separator();

    if (ImGui::Button("Die")) SetState("Die");

    ImGui::End();
}