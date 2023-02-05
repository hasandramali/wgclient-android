// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

/*
* Globals initialization
*/
#ifndef HOOK_GAMEDLL

bool s_tutorDisabledThisGame = false;
float s_nextCvarCheckTime = 0.0f;

cvar_t cv_tutor_message_repeats = { "_tutor_message_repeats", "5", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_debug_level = { "_tutor_debug_level", "0", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_view_distance = { "_tutor_view_distance", "1000", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_viewable_check_interval = { "_tutor_bomb_viewable_check_interval", "0.5", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_look_distance = { "_tutor_look_distance", "200", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_look_angle = { "_tutor_look_angle", "10", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_examine_time = { "_tutor_examine_time", "0.5", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_message_minimum_display_time = { "_tutor_message_minimum_display_time", "1", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_message_character_display_time_coefficient = { "_tutor_message_character_display_time_coefficient", "0.07", FCVAR_SERVER, 0.0f, NULL };
cvar_t cv_tutor_hint_interval_time = { "_tutor_hint_interval_time", "10.0", FCVAR_SERVER, 0.0f, NULL };

#endif

void InstallTutor(bool start)
{
	if (TheTutor != NULL)
	{
		delete TheTutor;
		TheTutor = NULL;
	}

	if (start)
	{
		TheTutor = new CCSTutor;
	}

	s_tutorDisabledThisGame = false;
	s_nextCvarCheckTime = gpGlobals->time;
}

void Tutor_RegisterCVars()
{
#ifdef REGAMEDLL_FIXES
	if (!g_bIsCzeroGame)
		return;
#endif

	CVAR_REGISTER(&cv_tutor_message_repeats);
	CVAR_REGISTER(&cv_tutor_debug_level);
	CVAR_REGISTER(&cv_tutor_view_distance);
	CVAR_REGISTER(&cv_tutor_viewable_check_interval);
	CVAR_REGISTER(&cv_tutor_look_distance);
	CVAR_REGISTER(&cv_tutor_look_angle);
	CVAR_REGISTER(&cv_tutor_examine_time);
	CVAR_REGISTER(&cv_tutor_message_minimum_display_time);
	CVAR_REGISTER(&cv_tutor_message_character_display_time_coefficient);
	CVAR_REGISTER(&cv_tutor_hint_interval_time);
}

void MonitorTutorStatus()
{
	static cvar_t *tutor_enable = NULL;
	static bool tutor_enableCvarExists = true;

	bool shouldTutorBeOn = false;
	int numHumans = 0;

	if (!tutor_enableCvarExists || s_nextCvarCheckTime > gpGlobals->time)
		return;

	if (tutor_enable != NULL || (tutor_enable = CVAR_GET_POINTER("tutor_enable")) != NULL)
	{
		if (!s_tutorDisabledThisGame && tutor_enable->value > 0.0f)
		{
			shouldTutorBeOn = true;
		}
	}
	else
		tutor_enableCvarExists = false;

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex(i);

		if (pPlayer && !pPlayer->IsBot())
			++numHumans;
	}

	if (shouldTutorBeOn)
	{
		if (numHumans <= 1)
		{
			if (TheTutor == NULL)
			{
				CBasePlayer *localPlayer = UTIL_GetLocalPlayer();

				if (localPlayer != NULL)
				{
					ClientPrint(localPlayer->pev, HUD_PRINTCENTER, "#CZero_Tutor_Turned_On");
				}

				TheTutor = new CCSTutor;
			}
		}
		else
			s_tutorDisabledThisGame = true;
	}
	else
	{
		if (TheTutor != NULL)
		{
			CBasePlayer *localPlayer = UTIL_GetLocalPlayer();

			if (localPlayer != NULL)
			{
				ClientPrint(localPlayer->pev, HUD_PRINTCENTER, "#CZero_Tutor_Turned_Off");
			}

			delete TheTutor;
			TheTutor = NULL;
		}
	}

	s_nextCvarCheckTime = gpGlobals->time + 1.0f;
}
