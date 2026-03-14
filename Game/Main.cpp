#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <SDL3/SDL_main.h>
#include <Minigin.h> 
#include <SceneManager.h>
#include <SceneLoader.h>
#include "GameComponentsRegistry.h" 

#if USE_STEAMWORKS
#include "Achievements/BurgerTimeAchievements.h"
#include "SteamAchievements/Achievement.h"
#endif
void LoadGame()
{

    dae::GameComponentsRegistry::RegisterAll();

#if USE_STEAMWORKS
    dae::g_SteamAchievements = new dae::CSteamAchievements(dae::g_Achievements, 4);
#endif

    auto& scene = dae::SceneManager::GetInstance().CreateScene();

    dae::SceneLoader::LoadScene(scene, "Levels/Prog4Ass.json");
}

int main(int, char* [])
{
#if __EMSCRIPTEN__
    std::filesystem::path data_location = "";
#else
    std::filesystem::path data_location = "./Data/";
#endif


    dae::Minigin engine(data_location);

    engine.Run(LoadGame);

#if USE_STEAMWORKS
    delete dae::g_SteamAchievements;
    dae::g_SteamAchievements = nullptr;
#endif

    return 0;
}