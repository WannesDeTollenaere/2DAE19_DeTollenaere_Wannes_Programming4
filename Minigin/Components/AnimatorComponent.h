#pragma once
#include "Component.h"
#include <string>
#include <map>
#include <SDL3/SDL.h>

namespace dae
{
    struct AnimationClip
    {
        int startRow;
        int startCol;
        int frameCount;
        float frameTime;
        bool isLooping;
    };

    class TextureComponent; 

    class AnimatorComponent final : public Component
    {
    public:
        AnimatorComponent(GameObject* owner, int frameWidth, int frameHeight);
        virtual ~AnimatorComponent() = default;

        void Update() override;

        void AddAnimation(const std::string& name, const AnimationClip& clip);
        void PlayAnimation(const std::string& name);

    private:
        TextureComponent* m_pTextureComponent{ nullptr };

        std::map<std::string, AnimationClip> m_animations;
        std::string m_currentAnimation;

        int m_frameWidth;
        int m_frameHeight;

        int m_currentFrameIdx{ 0 };
        float m_timer{ 0.0f };
    };
}