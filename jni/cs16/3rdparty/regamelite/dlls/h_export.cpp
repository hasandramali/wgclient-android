// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

//! Holds engine functionality callbacks
enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

// Receive engine function table from engine.
// This appears to be the _first_ DLL routine called by the engine, so we
// do some setup operations here.
C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t *pEnginefuncsTable, globalvars_t *pGlobals)
{
	Q_memcpy(&g_engfuncs, pEnginefuncsTable, sizeof(enginefuncs_t));
	gpGlobals = pGlobals;
	g_iXashEngineBuildNumber = CVAR_GET_FLOAT( "build" ); // 0 for old builds or GoldSrc

	Regamedll_Game_Init();
}
