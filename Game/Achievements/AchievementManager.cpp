//#include "AchievementManager.h"
//#include "ObserverSys/EventManager.h"
//#include "Events/ScoreChangedEvent.h"
//#include "sdbm_hash.h"
//#include "Achievements/BurgerTimeAchievements.h"
//
//namespace dae
//{
//    AchievementManager::AchievementManager()
//    {
//        EventManager::GetInstance().AttachEvent(make_sdbm_hash("ScoreChanged"), this);
//    }
//
//    void AchievementManager::HandleEvent(const Event* pEvent)
//    {
//        if (pEvent->id == make_sdbm_hash("ScoreChanged"))
//        {
//            if (const auto* pScoreEvent = dynamic_cast<const ScoreChangedEvent*>(pEvent))
//            {
//                if (pScoreEvent->newScore >= 500)
//                {
//#if USE_STEAMWORKS
//                    if (g_SteamAchievements != nullptr)
//                    {
//                        g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
//                    }
//#endif
//                }
//            }
//        }
//    }
//}