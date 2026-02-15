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

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

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

	scene.Add(std::move(textObj));

	// FPS CLOCK
	auto fpsTextObj = std::make_unique<dae::GameObject>();
	fpsTextObj->SetPosition(20, 20);

	auto fpsTextComp = fpsTextObj->AddComponent<dae::TextComponent>("60.0 fps", font);
	fpsTextComp->SetColor({ 255, 255, 255, 255 });

	scene.Add(std::move(fpsTextObj));
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
