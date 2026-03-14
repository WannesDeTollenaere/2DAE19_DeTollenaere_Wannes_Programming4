#pragma once
#if USE_STEAMWORKS
#include <stdint.h>
#include "SteamAchievements/Achievement.h"
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)

namespace dae
{
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	Achievement_t g_Achievements[] =
	{
		_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
	};

	CSteamAchievements* g_SteamAchievements = NULL;

}
#endif