#pragma once
#include "Commands/Command.h"
#include "Sound/ServiceLocator.h"

namespace dae
{
    class ToggleMuteCommand final : public Command
    {
    public:
        void Execute() override
        {
            ServiceLocator::GetSoundSystem().ToggleMute();
        }
    };
}