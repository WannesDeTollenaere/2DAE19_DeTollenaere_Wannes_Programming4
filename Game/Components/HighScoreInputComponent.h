#pragma once
#include "Component.h"
#include <string>

namespace dae
{
    class TextComponent; 

    class HighScoreInputComponent final : public Component
    {
    public:
        HighScoreInputComponent(GameObject* owner, int controllerIndex = 0);
        virtual ~HighScoreInputComponent() = default;

        void Update() override; 

        void NextLetter();
        void PreviousLetter();
        void NextSlot();
        void PreviousSlot();
        void DeleteLetter();
        void Submit();

    private:
        int m_Score;
        std::string m_Name{ "A" }; 
        int m_CurrentIndex{ 0 };

        bool m_Submitted{ false };
        int m_ControllerIndex;

        TextComponent* m_pTextComponent{ nullptr };

        void UpdateVisualText(); 
    };
}