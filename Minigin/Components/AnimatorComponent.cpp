#include "AnimatorComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "TextureComponent.h" 

dae::AnimatorComponent::AnimatorComponent(GameObject* owner, int frameWidth, int frameHeight)
    : Component(owner), m_frameWidth(frameWidth), m_frameHeight(frameHeight)
{
}

void dae::AnimatorComponent::AddAnimation(const std::string& name, const AnimationClip& clip)
{
    m_animations[name] = clip;
}

void dae::AnimatorComponent::PlayAnimation(const std::string& name)
{
    if (m_currentAnimation != name)
    {
        m_currentAnimation = name;
        m_currentFrameIdx = 0;
        m_timer = 0.0f;
    }
}

void dae::AnimatorComponent::Update()
{
    if (m_currentAnimation.empty() || m_animations.find(m_currentAnimation) == m_animations.end()) return;

    if (!m_pTextureComponent)
    {
        m_pTextureComponent = GetOwner()->GetComponent<TextureComponent>();
        if (!m_pTextureComponent) return; 
    }

    const auto& clip = m_animations[m_currentAnimation];
    m_timer += GameTime::GetInstance().GetDeltaTime();

    if (m_timer >= clip.frameTime)
    {
        m_timer -= clip.frameTime;
        m_currentFrameIdx++;

        if (m_currentFrameIdx >= clip.frameCount)
        {
            m_currentFrameIdx = clip.isLooping ? 0 : clip.frameCount - 1;
        }
    }

    int col = clip.startCol + m_currentFrameIdx;
    int row = clip.startRow;

    SDL_FRect srcRect{};
    srcRect.x = static_cast<float>(col * m_frameWidth);
    srcRect.y = static_cast<float>(row * m_frameHeight);
    srcRect.w = static_cast<float>(m_frameWidth);
    srcRect.h = static_cast<float>(m_frameHeight);

    m_pTextureComponent->SetSourceRect(srcRect);
}
