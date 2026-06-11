#include "AnimatorComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "TextureComponent.h" 
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class AnimatorComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int frameWidth = data.value("frameWidth", 16);
            int frameHeight = data.value("frameHeight", 16);

            auto animator = go->AddComponent<AnimatorComponent>(frameWidth, frameHeight);

            if (data.contains("animations") && data["animations"].is_object())
            {
                for (auto& [name, val] : data["animations"].items())
                {
                    dae::AnimationClip clip{};
                    clip.startRow = val.value("startRow", 0);
                    clip.startCol = val.value("startCol", 0);
                    clip.frameCount = val.value("frameCount", 1);
                    clip.frameTime = val.value("frameTime", 0.1f);
                    clip.isLooping = val.value("isLooping", true);

                    animator->AddAnimation(name, clip);
                }
            }

            std::string playOnStart = data.value("playOnStart", "");
            if (!playOnStart.empty())
            {
                animator->PlayAnimation(playOnStart);
            }
        }
    };

    REGISTER_COMPONENT_PARSER(AnimatorComponent, AnimatorComponentParser);
}


dae::AnimatorComponent::AnimatorComponent(GameObject* owner, int frameWidth, int frameHeight)
    : Component(owner), m_frameWidth(frameWidth), m_frameHeight(frameHeight)
{
    EXPOSE(m_frameWidth);
    EXPOSE(m_frameHeight);
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
