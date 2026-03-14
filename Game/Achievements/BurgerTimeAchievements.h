#pragma once
#if USE_STEAMWORKS
#include <stdint.h>
#include "SteamAchievements/Achievement.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable:4996)
#endif
#include <steam_api.h>
#ifdef _MSC_VER
#pragma warning (pop)
#endif

namespace dae
{
	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	inline Achievement_t g_Achievements[] =
	{
		_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
		_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
		_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
		_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
	};

	inline CSteamAchievements* g_SteamAchievements = nullptr;

}
#endif