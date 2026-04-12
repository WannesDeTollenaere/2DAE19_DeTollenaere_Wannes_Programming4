#pragma once
#include "Command.h"
#include <SDL3/SDL.h>

namespace dae {
    class QuitCommand final : public Command {
    public:
        void Execute() override {
            SDL_Event quitEvent;
            quitEvent.type = SDL_EVENT_QUIT;
            SDL_PushEvent(&quitEvent);
        }
    };
}