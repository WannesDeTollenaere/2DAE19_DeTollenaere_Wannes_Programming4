//#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
//
//#if _DEBUG && __has_include(<vld.h>)
//#include <vld.h>
//#endif
//
//#include "Minigin.h"
//#include "SceneManager.h"
//#include "ResourceManager.h"
//#include "Components/TextComponent.h"
//#include "Scene.h"
//#include "Components/TextureComponent.h"
//#include "Components/DynamicTextComponent.h"
//#include "GameTime.h"
//#include "Components/RotatorComponent.h"
//#include "Components/CacheProfilerComponent.h"
//#include "Components/AnimatorComponent.h"
//#include "SceneLoader.h"
//#include "InputManager.h"
//#include "Commands/MoveCommand.h"
//#include <sstream>
//#include <iomanip>
//#include "ComponentsRegistry.h"
//
//#include <filesystem>
//namespace fs = std::filesystem;
//
//
//
//static void load()
//{
//	dae::ComponentsRegistry::RegisterAll();
//
//	auto& scene = dae::SceneManager::GetInstance().CreateScene();
//	dae::SceneLoader::LoadScene(scene, "Levels/Prog4Ass.json");
//}
//
//int main(int, char*[]) {
//#if __EMSCRIPTEN__
//	fs::path data_location = "";
//#else
//	fs::path data_location = "./Data/";
//	if(!fs::exists(data_location))
//		data_location = "../Data/";
//#endif
//	dae::Minigin engine(data_location);
//	engine.Run(load);
//    return 0;
//}
