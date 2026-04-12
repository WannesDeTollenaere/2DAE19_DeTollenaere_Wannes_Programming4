#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

dae::InputManager::InputManager() : InputManager(2) {}

dae::InputManager::InputManager(uint16_t amountOfControllers)
{
	for (int i = 0; i < amountOfControllers; ++i)
	{
		m_Controllers.push_back(std::make_unique<Gamepad>(i));
	}
}

void dae::InputManager::BindCommand(uint16_t controllerIndex, Gamepad::ControllerButton button, InputState state, std::unique_ptr<Command> command)
{
	ControllerInput inputKey{ controllerIndex, button, state };

	m_ControllerCommands[inputKey] = std::move(command);
}

void dae::InputManager::UnbindCommand(uint16_t controllerIndex, Gamepad::ControllerButton button, InputState state)
{
	ControllerInput inputKey{ controllerIndex, button, state };
	m_ControllerCommands.erase(inputKey);
}


void dae::InputManager::BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command)
{
	KeyboardInput inputKey{ key, state };
	m_KeyboardCommands[inputKey] = std::move(command);
}

void dae::InputManager::UnbindKeyboardCommand(SDL_Scancode key, InputState state)
{
	KeyboardInput inputKey{ key, state };
	m_KeyboardCommands.erase(inputKey);
}

// --- Input Evaluation Helpers ---

bool dae::InputManager::NeedToExecuteCommand(const KeyboardInput& input) const
{
	bool isDownThisFrame = m_CurrentKeyboardState[input.key] && !m_PreviousKeyboardState[input.key];
	bool isUpThisFrame = !m_CurrentKeyboardState[input.key] && m_PreviousKeyboardState[input.key];
	bool isPressed = m_CurrentKeyboardState[input.key];

	return (input.state == InputState::Down && isDownThisFrame) ||
		(input.state == InputState::Up && isUpThisFrame) ||
		(input.state == InputState::Pressed && isPressed);
}

bool dae::InputManager::NeedToExecuteCommand(const ControllerInput& input) const
{
	if (input.controllerIndex >= m_Controllers.size()) return false;

	const auto& controller = m_Controllers[input.controllerIndex];

	return ((input.state == InputState::Down && controller->IsDown(input.button)) ||
		(input.state == InputState::Up && controller->IsUp(input.button)) ||
		(input.state == InputState::Pressed && controller->IsPressed(input.button)));
}

bool dae::InputManager::ProcessInput()
{
	int numKeys;
	const bool* pKeyboardState = SDL_GetKeyboardState(&numKeys);
	m_PreviousKeyboardState = m_CurrentKeyboardState;
	m_CurrentKeyboardState.assign(pKeyboardState, pKeyboardState + numKeys);

	if (m_PreviousKeyboardState.size() != m_CurrentKeyboardState.size()) {
		m_PreviousKeyboardState = m_CurrentKeyboardState;
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) return false;
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	for (auto& controller : m_Controllers) {
		controller->Update();
	}

	ProcessCommandMap(m_KeyboardCommands);
	ProcessCommandMap(m_ControllerCommands);

	return true;
}