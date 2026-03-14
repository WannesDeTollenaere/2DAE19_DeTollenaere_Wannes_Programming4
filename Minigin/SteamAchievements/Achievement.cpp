#if USE_STEAMWORKS
#include <stdint.h>
#include "Achievement.h"

#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)

#include <cstdio> 

namespace dae
{
    // THIS CODE IS ALL FROM: https://partner.steamgames.com/doc/features/achievements/ach_guide

    CSteamAchievements::CSteamAchievements(Achievement_t* Achievements, int NumAchievements) :
        m_iAppID(0),
        m_bInitialized(false),
        m_CallbackUserStatsReceived(this, &CSteamAchievements::OnUserStatsReceived),
        m_CallbackUserStatsStored(this, &CSteamAchievements::OnUserStatsStored),
        m_CallbackAchievementStored(this, &CSteamAchievements::OnAchievementStored)
    {
        m_iAppID = SteamUtils()->GetAppID();
        m_pAchievements = Achievements;
        m_iNumAchievements = NumAchievements;
        RequestStats();
    }

    CSteamAchievements::~CSteamAchievements()
    {
    }

    bool CSteamAchievements::RequestStats()
    {
        // Is Steam loaded? If not we can't get stats.
        if (nullptr == SteamUserStats() || nullptr == SteamUser())
        {
            return false;
        }
        // Is the user logged on?  If not we can't get stats.
        if (!SteamUser()->BLoggedOn())
        {
            return false;
        }
        // Request user stats.
        m_bInitialized = true;

        for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
        {
            Achievement_t& ach = m_pAchievements[iAch];

            SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);

            snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
                SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "name"));
            snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
                SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "desc"));
        }

        return true;
    }

    bool CSteamAchievements::SetAchievement(const char* ID)
    {
        // Have we received a call back from Steam yet?
        if (m_bInitialized)
        {
            SteamUserStats()->SetAchievement(ID);
            return SteamUserStats()->StoreStats();
        }
        // If not then we can't set achievements yet
        return false;
    }

    void CSteamAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
    {
        if (m_iAppID == pCallback->m_nGameID)
        {
            if (k_EResultOK == pCallback->m_eResult)
            {
                //OutputDebugString("Received stats and achievements from Steam\n");
                m_bInitialized = true;

                // load achievements
                for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
                {
                    Achievement_t& ach = m_pAchievements[iAch];

                    SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);

                    // Replaced _snprintf with snprintf for cross-platform compatibility
                    snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
                        SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
                            "name"));
                    snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
                        SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
                            "desc"));
                }
            }
            else
            {
                // char buffer[128];
                // snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
                // OutputDebugString(buffer);
            }
        }
    }

    void CSteamAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
    {
        if (m_iAppID == pCallback->m_nGameID)
        {
            if (!(k_EResultOK == pCallback->m_eResult))
            {
                // char buffer[128];
                // snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
                // OutputDebugString(buffer);
            }
        }
    }

    void CSteamAchievements::OnAchievementStored([[maybe_unused]] UserAchievementStored_t* pCallback)
    {
        // we may get callbacks for other games' stats arriving, ignore them
        //if (m_iAppID == pCallback->m_nGameID)
        //{
        //    OutputDebugString("Stored Achievement for Steam\n");
        //}
    }
}
#endif