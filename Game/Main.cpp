#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <SDL3/SDL_main.h>
#include <Minigin.h> 
#include <SceneManager.h>
#include <SceneLoader.h>
#include "GameComponentsRegistry.h" 
#include "rendere"
//#include "Achievements/AchievementManager.h"

#if USE_STEAMWORKS
#include "Achievements/BurgerTimeAchievements.h"
#include "SteamAchievements/Achievement.h"
#endif

//std::unique_ptr<dae::AchievementManager> g_AchievementManager;

void LoadGame()
{
    dae::GameComponentsRegistry::RegisterAll();

#if USE_STEAMWORKS
    dae::g_SteamAchievements = new dae::CSteamAchievements(dae::g_Achievements, 4);
#endif

    //g_AchievementManager = std::make_unique<dae::AchievementManager>();
    auto& scene = dae::SceneManager::GetInstance().CreateScene();

    dae::SceneLoader::LoadScene(scene, "Levels/level1.json");
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

    //g_AchievementManager.reset();
#if USE_STEAMWORKS
    delete dae::g_SteamAchievements;
    dae::g_SteamAchievements = nullptr;
#endif

    return 0;
}