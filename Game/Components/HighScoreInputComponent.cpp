#include "HighScoreInputComponent.h"
#include "Helpers/HighScoreManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Commands/HighScoreCommands.h"
#include "SceneManager.h"
#include <imgui.h>
#include <SDL3/SDL.h>
#include "Components/TextComponent.h"
#include "SceneLoader.h"

namespace dae
{
    REGISTER_COMPONENT_PARSER(HighScoreInputComponent, SimpleParser<HighScoreInputComponent>);

    HighScoreInputComponent::HighScoreInputComponent(GameObject* owner, int controllerIndex)
        : Component(owner), m_ControllerIndex(controllerIndex)
    {
        m_Score = GameManager::GetInstance().GetScore();
        auto& input = InputManager::GetInstance();

        input.BindCommand(static_cast<uint16_t>(m_ControllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Down, std::make_unique<ChangeLetterCommand>(owner, false));
        input.BindCommand(static_cast<uint16_t>(m_ControllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Down, std::make_unique<ChangeLetterCommand>(owner, true));
        input.BindCommand(static_cast<uint16_t>(m_ControllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Down, std::make_unique<MoveCursorCommand>(owner, true));
        input.BindCommand(static_cast<uint16_t>(m_ControllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Down, std::make_unique<MoveCursorCommand>(owner, false));

        input.BindCommand(static_cast<uint16_t>(m_ControllerIndex), Gamepad::ControllerButton::A, InputState::Down, std::make_unique<SubmitScoreCommand>(owner));
        input.BindCommand(static_cast<uint16_t>(m_ControllerIndex), Gamepad::ControllerButton::B, InputState::Down, std::make_unique<DeleteLetterCommand>(owner));

        // Keyboard
        input.BindKeyboardCommand(SDL_SCANCODE_UP, InputState::Down, std::make_unique<ChangeLetterCommand>(owner, false));
        input.BindKeyboardCommand(SDL_SCANCODE_DOWN, InputState::Down, std::make_unique<ChangeLetterCommand>(owner, true));
        input.BindKeyboardCommand(SDL_SCANCODE_RIGHT, InputState::Down, std::make_unique<MoveCursorCommand>(owner, true));
        input.BindKeyboardCommand(SDL_SCANCODE_LEFT, InputState::Down, std::make_unique<MoveCursorCommand>(owner, false));

        input.BindKeyboardCommand(SDL_SCANCODE_RETURN, InputState::Down, std::make_unique<SubmitScoreCommand>(owner));
        input.BindKeyboardCommand(SDL_SCANCODE_BACKSPACE, InputState::Down, std::make_unique<DeleteLetterCommand>(owner));
    }


    void HighScoreInputComponent::Update()
    {
        if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<TextComponent>();

        if (m_pTextComponent)
            UpdateVisualText(); 
            
    }

    void HighScoreInputComponent::NextLetter()
    {
        if (m_Submitted) return;
        char& c = m_Name[m_CurrentIndex];

        if (c == 'Z') c = ' ';      
        else if (c == ' ') c = 'A'; 
        else c++;                  

        UpdateVisualText();
    }

    void HighScoreInputComponent::PreviousLetter()
    {
        if (m_Submitted) return;
        char& c = m_Name[m_CurrentIndex];

        if (c == 'A') c = ' ';      
        else if (c == ' ') c = 'Z'; 
        else c--;                   

        UpdateVisualText();
    }

    void HighScoreInputComponent::NextSlot()
    {
        if (m_Submitted) return;
        m_CurrentIndex++;

        if (m_CurrentIndex >= static_cast<int>(m_Name.length()))
        {
            m_Name += 'A';
        }
        UpdateVisualText();
    }

    void HighScoreInputComponent::PreviousSlot()
    {
        if (m_Submitted) return;
        m_CurrentIndex--;
        if (m_CurrentIndex < 0) m_CurrentIndex = 0; 
        UpdateVisualText();
    }

    void HighScoreInputComponent::DeleteLetter()
    {
        if (m_Submitted || m_Name.length() <= 1) return;

        m_Name.erase(m_CurrentIndex, 1); 

        if (m_CurrentIndex >= static_cast<int>(m_Name.length()))
        {
            m_CurrentIndex = static_cast<int>(m_Name.length()) - 1;
        }
        UpdateVisualText();
    }

    void HighScoreInputComponent::Submit()
    {
        if (m_Submitted) return;
        m_Submitted = true;

        HighScoreManager::GetInstance().AddScore(m_Name, m_Score);

        GameManager::GetInstance().ResetAll();
        SceneManager::GetInstance().SetActiveScene("Levels/LoadingScene.json");
    }

    void HighScoreInputComponent::UpdateVisualText()
    {
        if (!m_pTextComponent || m_Submitted) return;

        std::string displayStr = "SCORE: " + std::to_string(m_Score) + "   NAME: ";

        for (int i = 0; i < static_cast<int>(m_Name.length()); ++i)
        {
            if (i == m_CurrentIndex)
            {
                displayStr += "[";
                displayStr += m_Name[i];
                displayStr += "]";
            }
            else
            {
                displayStr += " ";
                displayStr += m_Name[i];
                displayStr += " ";
            }
        }

        m_pTextComponent->SetText(displayStr);
    }
}