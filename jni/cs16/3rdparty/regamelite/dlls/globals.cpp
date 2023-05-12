// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

/*
* Globals initialization
*/
#ifndef HOOK_GAMEDLL

const Vector g_vecZero(0, 0, 0);
NOXREF u_long g_ulFrameCount = 0;

#endif

int g_Language;
DLL_GLOBAL int			gLevelLoaded;

NOXREF u_long g_ulModelIndexEyes;
Vector g_vecAttackDir;
int g_iSkillLevel;
int gDisplayTitle;
bool g_bIsCzeroGame = false;
bool g_bAllowedCSBot = false;
bool g_bHostageImprov = false;
DLL_GLOBAL bool		g_fPhysicInitialized = FALSE;
DLL_GLOBAL int		g_iXashEngineBuildNumber;
