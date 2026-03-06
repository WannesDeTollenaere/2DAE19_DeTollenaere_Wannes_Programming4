#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Components/TextComponent.h"
#include "Scene.h"
#include "Components/TextureComponent.h"
#include "Components/DynamicTextComponent.h"
#include "GameTime.h"
#include "Components/RotatorComponent.h"
#include "Components/CacheProfilerComponent.h"
#include "Components/AnimatorComponent.h"
#include "Prefab/PlayerPrefab.h"
#include "SceneLoader.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"
#include <sstream>
#include <iomanip>

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();

	//dae::SceneLoader::LoadScene(scene, "Levels/Prog4Ass.json");
	//dae::SceneLoader::LoadScene(scene, "Levels/level1.json");
	// BG
	auto go1 = std::make_unique<dae::GameObject>();
	auto textureComp1 = go1->AddComponent<dae::TextureComponent>();
	textureComp1->SetTexture("background.png");
	scene.Add(std::move(go1));

	//LOGO
	auto go2 = std::make_unique<dae::GameObject>();
	auto textureComp2 = go2->AddComponent<dae::TextureComponent>();
	textureComp2->SetTexture("logo.png");
	go2->SetPosition(358, 180);
	scene.Add(std::move(go2));

	// PROG 4 ASS
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto textObj = std::make_unique<dae::GameObject>();
	textObj->SetPosition(292, 20);

	auto textComp = textObj->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	textComp->SetColor({ 255, 255, 255, 255 });
	textObj->AddComponent<dae::TextureComponent>();

	scene.Add(std::move(textObj));

	// FPS CLOCK
	auto fpsObj = std::make_unique<dae::GameObject>();
	auto fpsText = fpsObj->AddComponent<dae::TextComponent>("60.0 fps", font);
	fpsText->SetColor({ 255, 255, 255, 255 });
	fpsObj->AddComponent<dae::TextureComponent>();



	auto dynamicFPS = fpsObj->AddComponent<dae::DynamicTextComponent>([]()
		{
			float dt = dae::GameTime::GetInstance().GetDeltaTime();
			if (dt <= 0.0f) return std::string("0");

			std::stringstream ss;
			ss << std::fixed << std::setprecision(1) << (1.0f / dt);
			return ss.str();
		});
	dynamicFPS->SetPostfix(" FPS");

	fpsObj->SetPosition(10.f, 10.f);
	scene.Add(std::move(fpsObj));



	// CHARCTERS
	/*auto centerAnchor = std::make_unique<dae::GameObject>();
	centerAnchor->SetPosition(200.f, 300.f);

	auto char1 = std::make_unique<dae::GameObject>();
	auto tex1 = char1->AddComponent<dae::TextureComponent>();
	tex1->SetTexture("pacman.png");
	char1->AddComponent<dae::RotatorComponent>(10.f, 10.f);
	char1->SetParent(centerAnchor.get(), false);

	auto char2 = std::make_unique<dae::GameObject>();
	auto tex2 = char2->AddComponent<dae::TextureComponent>();
	tex2->SetTexture("pacman.png");
	char2->AddComponent<dae::RotatorComponent>(60.f, 4.0f);
	char2->SetParent(char1.get(), false);

	scene.Add(std::move(centerAnchor));
	scene.Add(std::move(char1));
	scene.Add(std::move(char2));

	auto profiler = std::make_unique<dae::GameObject>();
	profiler->AddComponent<dae::CacheProfilerComponent>();

	scene.Add(std::move(profiler));*/


	auto char1 = std::make_unique<dae::GameObject>();
	char1->SetPosition(100.f, 200.f); 

	auto tex1 = char1->AddComponent<dae::TextureComponent>();
	tex1->SetTexture("pacman.png");



	auto char2 = std::make_unique<dae::GameObject>();
	char2->SetPosition(300.f, 200.f);

	auto tex2 = char2->AddComponent<dae::TextureComponent>();
	tex2->SetTexture("pacman.png");

	constexpr float baseSpeed = 50.f;
	constexpr float doubleSpeed = baseSpeed * 2.f; 

	input.BindKeyboardCommand(SDL_SCANCODE_W, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char1.get(), glm::vec2(0, -1), baseSpeed));
	input.BindKeyboardCommand(SDL_SCANCODE_S, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char1.get(), glm::vec2(0, 1), baseSpeed));
	input.BindKeyboardCommand(SDL_SCANCODE_A, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char1.get(), glm::vec2(-1, 0), baseSpeed));
	input.BindKeyboardCommand(SDL_SCANCODE_D, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char1.get(), glm::vec2(1, 0), baseSpeed));

	input.BindCommand(0, dae::Gamepad::ControllerButton::DPadUp, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char2.get(), glm::vec2(0, -1), doubleSpeed));
	input.BindCommand(0, dae::Gamepad::ControllerButton::DPadDown, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char2.get(), glm::vec2(0, 1), doubleSpeed));
	input.BindCommand(0, dae::Gamepad::ControllerButton::DPadLeft, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char2.get(), glm::vec2(-1, 0), doubleSpeed));
	input.BindCommand(0, dae::Gamepad::ControllerButton::DPadRight, dae::InputState::Pressed, std::make_unique<dae::MoveCommand>(char2.get(), glm::vec2(1, 0), doubleSpeed));

	scene.Add(std::move(char1));
	scene.Add(std::move(char2));
	// ANIMATION 
	//scene.Add(dae::PlayerPrefab::Create(100.f, 100.f));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
