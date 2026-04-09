#pragma once
#include "Commands/GameObjectCommand.h"
#include "Components/HighScoreInputComponent.h"

namespace dae
{
    class ChangeLetterCommand final : public GameObjectCommand
    {
    public:
        ChangeLetterCommand(GameObject* pOwner, bool next)
            : GameObjectCommand(pOwner), m_Next(next) {
        }

        void Execute() override
        {
            auto comp = GetGameObject()->GetComponent<HighScoreInputComponent>();
            if (comp)
            {
                if (m_Next) comp->NextLetter();
                else comp->PreviousLetter();
            }
        }
    private:
        bool m_Next;
    };
    class DeleteLetterCommand final : public GameObjectCommand
    {
    public:
        DeleteLetterCommand(GameObject* pOwner) : GameObjectCommand(pOwner) {}

        void Execute() override
        {
            auto comp = GetGameObject()->GetComponent<HighScoreInputComponent>();
            if (comp)
            {
                comp->DeleteLetter();
            }
        }
    };
    class MoveCursorCommand final : public GameObjectCommand
    {
    public:
        MoveCursorCommand(GameObject* pOwner, bool next)
            : GameObjectCommand(pOwner), m_Next(next) {
        }

        void Execute() override
        {
            auto comp = GetGameObject()->GetComponent<HighScoreInputComponent>();
            if (comp)
            {
                if (m_Next) comp->NextSlot();
                else comp->PreviousSlot();
            }
        }
    private:
        bool m_Next;
    };

    class SubmitScoreCommand final : public GameObjectCommand
    {
    public:
        SubmitScoreCommand(GameObject* pOwner) : GameObjectCommand(pOwner) {}

        void Execute() override
        {
            auto comp = GetGameObject()->GetComponent<HighScoreInputComponent>();
            if (comp)
            {
                comp->Submit();
            }
        }
    };
}