#pragma once
#include "Singleton.h"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <map>
#include "Commands/Command.h"
#include "Gamepad.h"

namespace dae
{
	enum class InputState : char
	{
		Down,
		Up,
		Pressed
	};

	struct ControllerInput
	{
		uint16_t controllerIndex;
		Gamepad::ControllerButton button;
		InputState state;

		auto operator<=>(const ControllerInput&) const = default;
	};

	struct KeyboardInput
	{
		SDL_Scancode key;
		InputState state;

		auto operator<=>(const KeyboardInput&) const = default;
	};

	class InputManager final : public Singleton<InputManager>
	{ 
	public:
		InputManager();
		InputManager(uint16_t amountOfControllers);
		~InputManager() = default;

		bool ProcessInput();
		void Clear() { m_KeyboardCommands.clear(); m_ControllerCommands.clear(); }

		void BindCommand(uint16_t controllerIndex, Gamepad::ControllerButton button, InputState state, std::unique_ptr<Command> command);
		void UnbindCommand(uint16_t controllerIndex, Gamepad::ControllerButton button, InputState state);

		void BindKeyboardCommand(SDL_Scancode key, InputState state, std::unique_ptr<Command> command);
		void UnbindKeyboardCommand(SDL_Scancode key, InputState state);

	private:
		std::vector<std::unique_ptr<Gamepad>> m_Controllers;

		std::map<ControllerInput, std::unique_ptr<Command>> m_ControllerCommands;
		std::map<KeyboardInput, std::unique_ptr<Command>> m_KeyboardCommands;

		std::vector<Uint8> m_PreviousKeyboardState;
		std::vector<Uint8> m_CurrentKeyboardState;


		bool NeedToExecuteCommand(const KeyboardInput& input) const;
		bool NeedToExecuteCommand(const ControllerInput& input) const;


		// controller or keyboard
		template <typename InputKeyType>
		void ProcessCommandMap(const std::map<InputKeyType, std::unique_ptr<Command>>& commands)
		{
			for (const auto& [input, command] : commands)
			{
				if (command && NeedToExecuteCommand(input))
				{
					command->Execute();
				}
			}
		}
	};
}