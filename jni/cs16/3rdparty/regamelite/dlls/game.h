/*
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at
*   your option) any later version.
*
*   This program is distributed in the hope that it will be useful, but
*   WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*   General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software Foundation,
*   Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*   In addition, as a special exception, the author gives permission to
*   link the code of this program with the Half-Life Game Engine ("HL
*   Engine") and Modified Game Libraries ("MODs") developed by Valve,
*   L.L.C ("Valve").  You must obey the GNU General Public License in all
*   respects for all of the code used other than the HL Engine and MODs
*   from Valve.  If you modify this file, you may extend this exception
*   to your version of the file, but you are not obligated to do so.  If
*   you do not wish to do so, delete this exception statement from your
*   version.
*
*/

#ifndef GAME_H
#define GAME_H
#ifdef _WIN32
#pragma once
#endif

#define LOG_ENEMYATTACK		1
#define LOG_TEAMMATEATTACK	2

// playerid
#define PLAYERID_MODE_EVERYONE	0
#define PLAYERID_MODE_TEAMONLY	1
#define PLAYERID_MODE_OFF	2

#define PLAYERID_EVERYONE 0
#define PLAYERID_TEAMONLY 1
#define PLAYERID_OFF 2

extern cvar_t *g_psv_gravity;
extern cvar_t *g_psv_aim;
extern cvar_t *g_footsteps;
extern cvar_t *g_psv_accelerate;
extern cvar_t *g_psv_friction;
extern cvar_t *g_psv_stopspeed;

extern cvar_t displaysoundlist;
extern cvar_t timelimit;
extern cvar_t flashlight;
extern cvar_t decalfrequency;
extern cvar_t fadetoblack;
extern cvar_t fragsleft;
extern cvar_t timeleft;
extern cvar_t friendlyfire;
extern cvar_t allowmonsters;
extern cvar_t roundtime;
extern cvar_t buytime;
extern cvar_t freezetime;
extern cvar_t c4timer;
extern cvar_t ghostfrequency;
extern cvar_t autokick;
extern cvar_t autokick_timeout;
extern cvar_t restartround;
extern cvar_t sv_restart;
extern cvar_t limitteams;
extern cvar_t autoteambalance;
extern cvar_t tkpunish;
extern cvar_t hostagepenalty;
extern cvar_t mirrordamage;
extern cvar_t logmessages;
extern cvar_t forcecamera;
extern cvar_t forcechasecam;
extern cvar_t mapvoteratio;
extern cvar_t logdetail;
extern cvar_t startmoney;
extern cvar_t maxrounds;
extern cvar_t winlimit;
extern cvar_t windifference;
extern cvar_t playerid;
extern cvar_t allow_spectators;
extern cvar_t mp_chattime;
extern cvar_t kick_percent;
extern cvar_t humans_join_team;
extern cvar_t sk_plr_9mm_bullet1;
extern cvar_t sk_plr_9mm_bullet2;
extern cvar_t sk_plr_9mm_bullet3;
extern cvar_t sk_plr_357_bullet1;
extern cvar_t sk_plr_357_bullet2;
extern cvar_t sk_plr_357_bullet3;
extern cvar_t sk_plr_9mmAR_bullet1;
extern cvar_t sk_plr_9mmAR_bullet2;
extern cvar_t sk_plr_9mmAR_bullet3;
extern cvar_t sk_plr_9mmAR_grenade1;
extern cvar_t sk_plr_9mmAR_grenade2;
extern cvar_t sk_plr_9mmAR_grenade3;
extern cvar_t sk_plr_buckshot1;
extern cvar_t sk_plr_buckshot2;
extern cvar_t sk_plr_buckshot3;
extern cvar_t sk_plr_rpg1;
extern cvar_t sk_plr_rpg2;
extern cvar_t sk_plr_rpg3;
extern cvar_t sk_12mm_bullet1;
extern cvar_t sk_12mm_bullet2;
extern cvar_t sk_12mm_bullet3;
extern cvar_t sk_9mmAR_bullet1;
extern cvar_t sk_9mmAR_bullet2;
extern cvar_t sk_9mmAR_bullet3;
extern cvar_t sk_9mm_bullet1;
extern cvar_t sk_9mm_bullet2;
extern cvar_t sk_9mm_bullet3;
extern cvar_t sk_suitcharger1;
extern cvar_t sk_suitcharger2;
extern cvar_t sk_suitcharger3;
extern cvar_t sk_battery1;
extern cvar_t sk_battery2;
extern cvar_t sk_battery3;
extern cvar_t sk_healthcharger1;
extern cvar_t sk_healthcharger2;
extern cvar_t sk_healthcharger3;
extern cvar_t sk_healthkit1;
extern cvar_t sk_healthkit2;
extern cvar_t sk_healthkit3;
extern cvar_t sk_scientist_heal1;
extern cvar_t sk_scientist_heal2;
extern cvar_t sk_scientist_heal3;

#ifdef REGAMEDLL_ADD

extern cvar_t maxmoney;
extern cvar_t round_infinite;
extern cvar_t hegrenade_penetration;
extern cvar_t nadedrops;
extern cvar_t roundrespawn_time;
extern cvar_t auto_reload_weapons;
extern cvar_t refill_bpammo_weapons;
extern cvar_t freeforall;
extern cvar_t auto_join_team;
extern cvar_t max_teamkills;
extern cvar_t fraglimit;
extern cvar_t round_restart_delay;

extern cvar_t showtriggers;
extern cvar_t hostagehurtable;
extern cvar_t roundover;
extern cvar_t forcerespawn;

#endif

void GameDLLInit();

#endif // GAME_H
