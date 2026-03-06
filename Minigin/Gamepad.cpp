#include "Gamepad.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
namespace dae
{
	class Gamepad::GamepadImpl
	{
		uint16_t m_ControllerIndex;
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		uint16_t m_ButtonsChanges{};
		uint16_t m_ButtonsPressedThisFrame{};
		uint16_t m_ButtonsReleasedThisFrame{};

	public:
		GamepadImpl(uint16_t controllerIndex) : m_ControllerIndex(controllerIndex)
		{
			ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		}

		void Update()
		{
			CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(m_ControllerIndex, &m_CurrentState);

			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
		}

		bool IsDown(ControllerButton button) const { return m_ButtonsPressedThisFrame & static_cast<uint16_t>(button); }
		bool IsUp(ControllerButton button) const { return m_ButtonsReleasedThisFrame & static_cast<uint16_t>(button); }
		bool IsPressed(ControllerButton button) const { return m_CurrentState.Gamepad.wButtons & static_cast<uint16_t>(button); }
	};
}
#else
#include <SDL3/SDL.h>

namespace dae
{
	class Gamepad::GamepadImpl
	{
		int m_ControllerIndex;
		SDL_Gamepad* m_pGamepad = nullptr;

		uint16_t m_PreviousState{};
		uint16_t m_CurrentState{};
		uint16_t m_ButtonsPressedThisFrame{};
		uint16_t m_ButtonsReleasedThisFrame{};

		SDL_GamepadButton MapControllerButtonToSDLButton(ControllerButton button) const {
			switch (button) {
			case ControllerButton::DPadUp: return SDL_GAMEPAD_BUTTON_DPAD_UP;
			case ControllerButton::DPadDown: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
			case ControllerButton::DPadLeft: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
			case ControllerButton::DPadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
			case ControllerButton::Start: return SDL_GAMEPAD_BUTTON_START;
			case ControllerButton::Back: return SDL_GAMEPAD_BUTTON_BACK;
			case ControllerButton::LeftThumb: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
			case ControllerButton::RightThumb: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
			case ControllerButton::LeftShoulder: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
			case ControllerButton::RightShoulder: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
			case ControllerButton::A: return SDL_GAMEPAD_BUTTON_SOUTH;
			case ControllerButton::B: return SDL_GAMEPAD_BUTTON_EAST;
			case ControllerButton::X: return SDL_GAMEPAD_BUTTON_WEST;
			case ControllerButton::Y: return SDL_GAMEPAD_BUTTON_NORTH;
			default: return SDL_GAMEPAD_BUTTON_INVALID;
			}
		}

	public:
		GamepadImpl(int controllerIndex) : m_ControllerIndex(controllerIndex)
		{
			SDL_InitSubSystem(SDL_INIT_GAMEPAD);

			int numJoysticks = 0;
			SDL_JoystickID* joysticks = SDL_GetJoysticks(&numJoysticks);
			if (joysticks && m_ControllerIndex < numJoysticks) {
				m_pGamepad = SDL_OpenGamepad(joysticks[m_ControllerIndex]);
			}
			SDL_free(joysticks);
		}

		~GamepadImpl()
		{
			if (m_pGamepad) SDL_CloseGamepad(m_pGamepad);
		}

		void Update()
		{
			m_PreviousState = m_CurrentState;
			m_CurrentState = 0;

			if (!m_pGamepad)
			{
				int numJoysticks = 0;
				SDL_JoystickID* joysticks = SDL_GetJoysticks(&numJoysticks);
				if (joysticks && m_ControllerIndex < numJoysticks) {
					m_pGamepad = SDL_OpenGamepad(joysticks[m_ControllerIndex]);
				}
				SDL_free(joysticks);

				if (!m_pGamepad) return;
			}

			const ControllerButton allButtons[] = {
				ControllerButton::DPadUp, ControllerButton::DPadDown, ControllerButton::DPadLeft, ControllerButton::DPadRight,
				ControllerButton::Start, ControllerButton::Back, ControllerButton::LeftThumb, ControllerButton::RightThumb,
				ControllerButton::LeftShoulder, ControllerButton::RightShoulder,
				ControllerButton::A, ControllerButton::B, ControllerButton::X, ControllerButton::Y
			};

			for (auto btn : allButtons) {
				if (SDL_GetGamepadButton(m_pGamepad, MapControllerButtonToSDLButton(btn))) {
					m_CurrentState |= static_cast<uint16_t>(btn);
				}
			}

			uint16_t buttonChanges = m_CurrentState ^ m_PreviousState;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState);
		}

		bool IsDown(ControllerButton button) const { return m_ButtonsPressedThisFrame & static_cast<uint16_t>(button); }
		bool IsUp(ControllerButton button) const { return m_ButtonsReleasedThisFrame & static_cast<uint16_t>(button); }
		bool IsPressed(ControllerButton button) const { return m_CurrentState & static_cast<uint16_t>(button); }
	};
}
#endif
namespace dae{
	Gamepad::Gamepad(uint16_t controllerIndex) : pImpl(std::make_unique<GamepadImpl>(controllerIndex)) {}
	Gamepad::~Gamepad() = default;
	void Gamepad::Update() { pImpl->Update(); }
	bool Gamepad::IsDown(ControllerButton button) const { return pImpl->IsDown(button); }
	bool Gamepad::IsUp(ControllerButton button) const { return pImpl->IsUp(button); }
	bool Gamepad::IsPressed(ControllerButton button) const { return pImpl->IsPressed(button); }
}

