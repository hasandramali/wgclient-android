// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

void Regamedll_Game_Init()
{
	g_bIsCzeroGame = UTIL_IsGame("czero");
	g_bAllowedCSBot = UTIL_AreBotsAllowed();		// determine whether bots can be used or not
	g_bHostageImprov = UTIL_AreHostagesImprov();		// determine whether hostage improv can be used or not

	WeaponInfoReset();
}
