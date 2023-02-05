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

#ifndef PLAYER_H
#define PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "pm_materials.h"
#include "hintmessage.h"
#include "unisignals.h"
#include "weapons.h"

#ifdef REGAMEDLL_ADD
#define MIN_BUY_TIME			0
#else
#define MIN_BUY_TIME			15	// the minimum threshold values for cvar mp_buytime 15 sec's
#endif

#define MAX_BUFFER_MENU			175
#define MAX_BUFFER_MENU_BRIEFING	50

#define MAX_PLAYER_NAME_LENGTH		32
#define MAX_AUTOBUY_LENGTH		256
#define MAX_REBUY_LENGTH		256

#define MAX_RECENT_PATH			20
#define MAX_HOSTAGE_ICON		4	// the maximum number of icons of the hostages in the HUD

#define SUITUPDATETIME			3.5
#define SUITFIRSTUPDATETIME		0.1

#define PLAYER_FATAL_FALL_SPEED		1100.0f
#define PLAYER_MAX_SAFE_FALL_SPEED	500.0f
#define PLAYER_USE_RADIUS		64.0f

#define ARMOR_RATIO			0.5	// Armor Takes 50% of the damage
#define ARMOR_BONUS			0.5	// Each Point of Armor is work 1/x points of health

#define FLASH_DRAIN_TIME		1.2	// 100 units/3 minutes
#define FLASH_CHARGE_TIME		0.2	// 100 units/20 seconds  (seconds per unit)

// damage per unit per second.
#define DAMAGE_FOR_FALL_SPEED		100.0f / (PLAYER_FATAL_FALL_SPEED - PLAYER_MAX_SAFE_FALL_SPEED)
#define PLAYER_MIN_BOUNCE_SPEED		350.0f

// won't punch player's screen/make scrape noise unless player falling at least this fast.
#define PLAYER_FALL_PUNCH_THRESHHOLD	250.0f

// Money blinks few of times on the freeze period
// NOTE: It works for CZ
#define MONEY_BLINK_AMOUNT		30

// Player physics flags bits
// CBasePlayer::m_afPhysicsFlags
#define PFLAG_ONLADDER			(1<<0)
#define PFLAG_ONSWING			(1<<0)
#define PFLAG_ONTRAIN			(1<<1)
#define PFLAG_ONBARNACLE		(1<<2)
#define PFLAG_DUCKING			(1<<3)	// In the process of ducking, but totally squatted yet
#define PFLAG_USING			(1<<4)	// Using a continuous entity
#define PFLAG_OBSERVER			(1<<5)	// player is locked in stationary cam mode. Spectators can move, observers can't.

#define TRAIN_OFF			0x00
#define TRAIN_NEUTRAL			0x01
#define TRAIN_SLOW			0x02
#define TRAIN_MEDIUM			0x03
#define TRAIN_FAST			0x04
#define TRAIN_BACK			0x05

#define TRAIN_ACTIVE			0x80
#define TRAIN_NEW			0xc0

#define SIGNAL_BUY			(1<<0)
#define SIGNAL_BOMB			(1<<1)
#define SIGNAL_RESCUE			(1<<2)
#define SIGNAL_ESCAPE			(1<<3)
#define SIGNAL_VIPSAFETY		(1<<4)

#define IGNOREMSG_NONE			0
#define IGNOREMSG_ENEMY			1
#define IGNOREMSG_TEAM			2

// max of 4 suit sentences queued up at any time
#define CSUITPLAYLIST			4

#define SUIT_GROUP			TRUE
#define SUIT_SENTENCE			FALSE

#define SUIT_REPEAT_OK			0
#define SUIT_NEXT_IN_30SEC		30
#define SUIT_NEXT_IN_1MIN		60
#define SUIT_NEXT_IN_5MIN		300
#define SUIT_NEXT_IN_10MIN		600
#define SUIT_NEXT_IN_30MIN		1800
#define SUIT_NEXT_IN_1HOUR		3600

#define TEAM_NAME_LENGTH		16

#define MAX_ID_RANGE			2048.0f
#define MAX_SPECTATOR_ID_RANGE		8192.0f
#define SBAR_STRING_SIZE		128

#define SBAR_TARGETTYPE_TEAMMATE	1
#define SBAR_TARGETTYPE_ENEMY		2
#define SBAR_TARGETTYPE_HOSTAGE		3

#define CHAT_INTERVAL			1.0f
#define CSUITNOREPEAT			32

#define AUTOAIM_2DEGREES		0.0348994967025
#define AUTOAIM_5DEGREES		0.08715574274766
#define AUTOAIM_8DEGREES		0.1391731009601
#define AUTOAIM_10DEGREES		0.1736481776669

#define SOUND_FLASHLIGHT_ON		"items/flashlight1.wav"
#define SOUND_FLASHLIGHT_OFF		"items/flashlight1.wav"

// custom enum
enum RewardType
{
	RT_NONE,
	RT_ROUND_BONUS,
	RT_PLAYER_RESET,
	RT_PLAYER_JOIN,
	RT_PLAYER_SPEC_JOIN,
	RT_PLAYER_BOUGHT_SOMETHING,
	RT_HOSTAGE_TOOK,
	RT_HOSTAGE_RESCUED,
	RT_HOSTAGE_DAMAGED,
	RT_HOSTAGE_KILLED,
	RT_TEAMMATES_KILLED,
	RT_ENEMY_KILLED,
	RT_INTO_GAME,
	RT_VIP_KILLED,
	RT_VIP_RESCUED_MYSELF
};

enum PLAYER_ANIM
{
	PLAYER_IDLE,
	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,
	PLAYER_ATTACK2,
	PLAYER_FLINCH,
	PLAYER_LARGE_FLINCH,
	PLAYER_RELOAD,
	PLAYER_HOLDBOMB
};

enum _Menu
{
	Menu_OFF,
	Menu_ChooseTeam,
	Menu_IGChooseTeam,
	Menu_ChooseAppearance,
	Menu_Buy,
	Menu_BuyPistol,
	Menu_BuyRifle,
	Menu_BuyMachineGun,
	Menu_BuyShotgun,
	Menu_BuySubMachineGun,
	Menu_BuyItem,
	Menu_Radio1,
	Menu_Radio2,
	Menu_Radio3,
	Menu_ClientBuy
};

enum TeamName
{
	UNASSIGNED,
	TERRORIST,
	CT,
	SPECTATOR,
};

enum ModelName
{
	MODEL_UNASSIGNED,
	MODEL_URBAN,
	MODEL_TERROR,
	MODEL_LEET,
	MODEL_ARCTIC,
	MODEL_GSG9,
	MODEL_GIGN,
	MODEL_SAS,
	MODEL_GUERILLA,
	MODEL_VIP,
	MODEL_MILITIA,
	MODEL_SPETSNAZ,
	MODEL_AUTO
};

enum JoinState
{
	JOINED,
	SHOWLTEXT,
	READINGLTEXT,
	SHOWTEAMSELECT,
	PICKINGTEAM,
	GETINTOGAME
};

enum TrackCommands
{
	CMD_SAY = 0,
	CMD_SAYTEAM,
	CMD_FULLUPDATE,
	CMD_VOTE,
	CMD_VOTEMAP,
	CMD_LISTMAPS,
	CMD_LISTPLAYERS,
	CMD_NIGHTVISION,
	COMMANDS_TO_TRACK,
};

struct RebuyStruct
{
	int m_primaryWeapon;
	int m_primaryAmmo;
	int m_secondaryWeapon;
	int m_secondaryAmmo;
	int m_heGrenade;
	int m_flashbang;
	int m_smokeGrenade;
	int m_defuser;
	int m_nightVision;
	ArmorType m_armor;
};

enum ThrowDirection
{
	THROW_NONE,
	THROW_FORWARD,
	THROW_BACKWARD,
	THROW_HITVEL,
	THROW_BOMB,
	THROW_GRENADE,
	THROW_HITVEL_MINUS_AIRVEL
};

enum sbar_data
{
	SBAR_ID_TARGETTYPE = 1,
	SBAR_ID_TARGETNAME,
	SBAR_ID_TARGETHEALTH,
	SBAR_END
};

enum MusicState { SILENT, CALM, INTENSE };

class CCSPlayer;

class CStripWeapons: public CPointEntity {
public:
	virtual void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

#ifdef HOOK_GAMEDLL

	void Use_(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

#endif

};

// Multiplayer intermission spots.
class CInfoIntermission: public CPointEntity {
public:
	virtual void Spawn();
	virtual void Think();

#ifdef HOOK_GAMEDLL

	void Spawn_();
	void Think_();

#endif

};

// Dead HEV suit prop
class CDeadHEV: public CBaseMonster {
public:
	virtual void Spawn();
	virtual void KeyValue(KeyValueData *pkvd);
	virtual int Classify();

#ifdef HOOK_GAMEDLL

	void Spawn_();
	void KeyValue_(KeyValueData *pkvd);
	int Classify_();

#endif

public:
	int m_iPose;				// which sequence to display -- temporary, don't need to save
	static char *m_szPoses[4];
};

class CSprayCan: public CBaseEntity {
public:
	virtual void Think();
	virtual int ObjectCaps() { return FCAP_DONT_SAVE; }

#ifdef HOOK_GAMEDLL

	void Think_();

#endif

public:
	void Spawn(entvars_t *pevOwner);
};

class CBloodSplat: public CBaseEntity {
public:
	void Spawn(entvars_t *pevOwner);
	void Spray();
};

class CBasePlayer: public CBaseMonster {
public:
	virtual void Spawn();
	virtual void Precache();
	virtual int Save(CSave &save);
	virtual int Restore(CRestore &restore);
	virtual int ObjectCaps();
	virtual int Classify();
	virtual void TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	virtual BOOL TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);
	virtual BOOL TakeHealth(float flHealth, int bitsDamageType);
	virtual void Killed(entvars_t *pevAttacker, int iGib);
	virtual void AddPoints(int score, BOOL bAllowNegativeScore);
	virtual void AddPointsToTeam(int score, BOOL bAllowNegativeScore);
	virtual BOOL AddPlayerItem(CBasePlayerItem *pItem);
	virtual BOOL RemovePlayerItem(CBasePlayerItem *pItem);
	virtual int GiveAmmo(int iAmount, char *szName, int iMax = -1);
	virtual void StartSneaking() { m_tSneaking = gpGlobals->time - 1; }

#ifndef REGAMEDLL_FIXES
	virtual void StopSneaking() { m_tSneaking = gpGlobals->time + 30; }
#else
	virtual void UpdateOnRemove();
#endif

	virtual BOOL IsSneaking() { return m_tSneaking <= gpGlobals->time; }
	virtual BOOL IsAlive() { return (pev->deadflag == DEAD_NO && pev->health > 0.0f); }
	virtual BOOL IsPlayer() { return (pev->flags & FL_SPECTATOR) != FL_SPECTATOR; }
	virtual BOOL IsNetClient() { return TRUE; }
	virtual const char *TeamID();
	virtual BOOL FBecomeProne();
	virtual Vector BodyTarget(const Vector &posSrc) { return Center() + pev->view_ofs * RANDOM_FLOAT(0.5, 1.1); }
	virtual int Illumination();
	virtual BOOL ShouldFadeOnDeath() { return FALSE; }
	virtual void ResetMaxSpeed();
	virtual void Jump();
	virtual void Duck();
	virtual void PreThink();
	virtual void PostThink();
	virtual Vector GetGunPosition();
	virtual BOOL IsBot() { return FALSE; }
	virtual void UpdateClientData();
	virtual void ImpulseCommands();
	virtual void RoundRespawn();
	virtual Vector GetAutoaimVector(float flDelta);
	virtual void Blind(float flUntilTime, float flHoldTime, float flFadeTime, int iAlpha);
	virtual void OnTouchingWeapon(CWeaponBox *pWeapon) { }

#if defined(REGAMEDLL_API) || defined(HOOK_GAMEDLL)
	void Spawn_();
	void Precache_();
	int ObjectCaps_();
	int Classify_();
	int Save_(CSave &save);
	int Restore_(CRestore &restore);
	void TraceAttack_(entvars_t *pevAttacker, float flDamage, VectorRef vecDir, TraceResult *ptr, int bitsDamageType);
	BOOL TakeDamage_(entvars_t *pevInflictor, entvars_t *pevAttacker, FloatRef flDamage, int bitsDamageType);
	BOOL TakeHealth_(float flHealth, int bitsDamageType);
	void Killed_(entvars_t *pevAttacker, int iGib);
	void AddPoints_(int score, BOOL bAllowNegativeScore);
	void AddPointsToTeam_(int score, BOOL bAllowNegativeScore);
	BOOL AddPlayerItem_(CBasePlayerItem *pItem);	
	BOOL RemovePlayerItem_(CBasePlayerItem *pItem);
	int GiveAmmo_(int iAmount, char *szName, int iMax);
	void ResetMaxSpeed_();
	void Jump_();
	void Duck_();
	void PreThink_();
	void PostThink_();
	void UpdateClientData_();
	void ImpulseCommands_();
	void RoundRespawn_();
	void Blind_(float flUntilTime, float flHoldTime, float flFadeTime, int iAlpha);
	const char *TeamID_();
	BOOL FBecomeProne_();
	int Illumination_();
	Vector GetGunPosition_();
	Vector GetAutoaimVector_(float flDelta);
#endif

public:
	static CBasePlayer *Instance(edict_t *pent) { return (CBasePlayer *)GET_PRIVATE(pent ? pent : ENT(0)); }
	static CBasePlayer *Instance(entvars_t *pev) { return Instance(ENT(pev)); }
	static CBasePlayer *Instance(int offset) { return Instance(ENT(offset)); }

	void SpawnClientSideCorpse();
	void Observer_FindNextPlayer(bool bReverse, const char *name = NULL);
	CBasePlayer *Observer_IsValidTarget(int iPlayerIndex, bool bSameTeam);
	CBasePlayer *Observer_IsValidTarget_(int iPlayerIndex, bool bSameTeam);

	void Disconnect();
	void Observer_Think();
	void Observer_HandleButtons();
	void Observer_SetMode(int iMode);
	void Observer_CheckTarget();
	void Observer_CheckProperties();
	int IsObserver() { return pev->iuser1; }
	void PlantC4();
	void Radio(const char *msg_id, const char *msg_verbose = NULL, short pitch = 100, bool showIcon = true);
	void Radio_(const char *msg_id, const char *msg_verbose = NULL, short pitch = 100, bool showIcon = true);
	CBasePlayer *GetNextRadioRecipient(CBasePlayer *pStartPlayer);
	void SmartRadio();
	void ThrowWeapon(char *pszItemName);
	void ThrowPrimary();
	void AddAccount(int amount, RewardType type = RT_NONE, bool bTrackChange = true);
	void AddAccount_(int amount, RewardType type = RT_NONE, bool bTrackChange = true);
	void Disappear();
	void Disappear_();
	void MakeVIP();
	void MakeVIP_();
	bool CanPlayerBuy(bool display = false);
	void SwitchTeam();
	void TabulateAmmo();
	void Pain(int iLastHitGroup, bool bHasArmour);
	BOOL IsBombGuy();
	bool IsLookingAtPosition(Vector *pos, float angleTolerance = 20.0f);
	void Reset();
	void SetScoreboardAttributes(CBasePlayer *destination = NULL);
	void RenewItems();
	void PackDeadPlayerItems();
	void GiveDefaultItems();
	void GiveDefaultItems_();
	void RemoveAllItems(BOOL removeSuit);
	void SetBombIcon(BOOL bFlash = FALSE);
	void SetProgressBarTime(int time);
	void SetProgressBarTime2(int time, float timeElapsed);
	void SetPlayerModel(BOOL HasC4);
	bool SetClientUserInfoName(char *infobuffer, char *szNewName);
	bool SetClientUserInfoName_(char *infobuffer, char *szNewName);
	void SetClientUserInfoModel(char *infobuffer, char *szNewModel);
	void SetClientUserInfoModel_api(char *infobuffer, char *szNewModel);
	void SetNewPlayerModel(const char *modelName);
	BOOL SwitchWeapon(CBasePlayerItem *pWeapon);
	void CheckPowerups();
	bool CanAffordPrimary();
	bool CanAffordPrimaryAmmo();
	bool CanAffordSecondaryAmmo();
	bool CanAffordArmor();
	bool CanAffordDefuseKit();
	bool CanAffordGrenade();
	bool NeedsPrimaryAmmo();
	bool NeedsSecondaryAmmo();
	bool NeedsArmor();
	bool NeedsDefuseKit();
	bool NeedsGrenade();
	BOOL IsOnLadder();
	BOOL FlashlightIsOn();
	void FlashlightTurnOn();
	void FlashlightTurnOff();
	void UpdatePlayerSound();
	void DeathSound();
	void SetAnimation(PLAYER_ANIM playerAnim);
	void SetAnimation_(PLAYER_ANIM playerAnim);
	void SetWeaponAnimType(const char *szExtention) { Q_strcpy(m_szAnimExtention, szExtention); }
	void CheatImpulseCommands(int iImpulse);
	void StartDeathCam();
	void StartObserver(Vector &vecPosition, Vector &vecViewAngle);
	void StartObserver_(Vector &vecPosition, Vector &vecViewAngle);
	void HandleSignals();
	void DropPlayerItem(const char *pszItemName);
	void DropPlayerItem_(const char *pszItemName);
	bool HasPlayerItem(CBasePlayerItem *pCheckItem);
	bool HasNamedPlayerItem(const char *pszItemName);
	bool HasWeapons();
	void SelectPrevItem(int iItem);
	void SelectNextItem(int iItem);
	void SelectLastItem();
	void SelectItem(const char *pstr);
	void ItemPreFrame();
	void ItemPostFrame();
	CBaseEntity *GiveNamedItem(const char *pszName);
	CBaseEntity *GiveNamedItem_(const char *pszName);
	CBaseEntity *GiveNamedItemEx(const char *pszName);
	void EnableControl(BOOL fControl);
	bool HintMessage(const char *pMessage, BOOL bDisplayIfPlayerDead = FALSE, BOOL bOverride = FALSE);
	void SendAmmoUpdate();
	void SendFOV(int fov);
	void WaterMove();
	void EXPORT PlayerDeathThink();
	void PlayerRespawnThink();
	void PlayerUse();
	void HostageUsed();
	void JoiningThink();
	void RemoveLevelText();
	void MenuPrint(const char *msg);
	void ResetMenu();
	void SyncRoundTimer();
	void CheckSuitUpdate();
	void SetSuitUpdate(char *name = NULL, int fgroup = 0, int iNoRepeatTime = 0);
	void UpdateGeigerCounter();
	void CheckTimeBasedDamage();
	void BarnacleVictimBitten(entvars_t *pevBarnacle);
	void BarnacleVictimReleased();
	static int GetAmmoIndex(const char *psz);
	int AmmoInventory(int iAmmoIndex);
	void ResetAutoaim();
	Vector AutoaimDeflection(Vector &vecSrc, float flDist, float flDelta);
	void ForceClientDllUpdate();
	void DeathMessage(entvars_t *pevAttacker) {};
	void SetCustomDecalFrames(int nFrames);
	int GetCustomDecalFrames();
	void InitStatusBar();
	void UpdateStatusBar();
	void StudioEstimateGait();
	void StudioPlayerBlend(int *pBlend, float *pPitch);
	void CalculatePitchBlend();
	void CalculateYawBlend();
	void StudioProcessGait();
	void SendHostagePos();
	void SendHostageIcons();
	void ResetStamina();
	BOOL IsArmored(int nHitGroup);
	BOOL ShouldDoLargeFlinch(int nHitGroup, int nGunType);
	void SetPrefsFromUserinfo(char *infobuffer);
	void SendWeatherInfo();
	void UpdateShieldCrosshair(bool draw);
	bool HasShield();
	bool IsProtectedByShield() { return HasShield() && m_bShieldDrawn; }
	void RemoveShield();
	void DropShield(bool bDeploy = true);
	void DropShield_(bool bDeploy = true);
	void GiveShield(bool bDeploy = true);
	void GiveShield_(bool bDeploy = true);
	bool IsHittingShield(Vector &vecDirection, TraceResult *ptr);
	bool SelectSpawnSpot(const char *pEntClassName, CBaseEntity* &pSpot);
	bool IsReloading() const;
	bool IsBlind() const { return (m_blindUntilTime > gpGlobals->time); }
	bool IsAutoFollowAllowed() const { return (gpGlobals->time > m_allowAutoFollowTime); }
	void InhibitAutoFollow(float duration) { m_allowAutoFollowTime = gpGlobals->time + duration; }
	void AllowAutoFollow() { m_allowAutoFollowTime = 0; }
	void ClearAutoBuyData();
	void AddAutoBuyData(const char *str);
	void AutoBuy();
	void ClientCommand(const char *cmd, const char *arg1 = NULL, const char *arg2 = NULL, const char *arg3 = NULL);
	void PrioritizeAutoBuyString(char *autobuyString, const char *priorityString);
	const char *PickPrimaryCareerTaskWeapon();
	const char *PickSecondaryCareerTaskWeapon();
	const char *PickFlashKillWeaponString();
	const char *PickGrenadeKillWeaponString();
	bool ShouldExecuteAutoBuyCommand(AutoBuyInfoStruct *commandInfo, bool boughtPrimary, bool boughtSecondary);
	void PostAutoBuyCommandProcessing(AutoBuyInfoStruct *commandInfo, bool &boughtPrimary, bool &boughtSecondary);
	void ParseAutoBuyString(const char *string, bool &boughtPrimary, bool &boughtSecondary);
	AutoBuyInfoStruct *GetAutoBuyCommandInfo(const char *command);
	void InitRebuyData(const char *str);
	void BuildRebuyStruct();
	void Rebuy();
	void RebuyPrimaryWeapon();
	void RebuyPrimaryAmmo();
	void RebuySecondaryWeapon();
	void RebuySecondaryAmmo();
	void RebuyHEGrenade();
	void RebuyFlashbang();
	void RebuySmokeGrenade();
	void RebuyDefuser();
	void RebuyNightVision();
	void RebuyArmor();
	void UpdateLocation(bool forceUpdate = false);
	void SetObserverAutoDirector(bool val) { m_bObserverAutoDirector = val; }
	bool IsObservingPlayer(CBasePlayer *pPlayer);
	bool CanSwitchObserverModes() const { return m_canSwitchObserverModes; }
	void SendItemStatus();
	edict_t *EntSelectSpawnPoint();
	void SetScoreAttrib(CBasePlayer *dest);
	void ReloadWeapons(CBasePlayerItem *pWeapon = nullptr, bool bForceReload = false, bool bForceRefill = false);
	void TeamChangeUpdate();
	bool HasRestrictItem(ItemID item, ItemRestType type);
	bool HasRestrictItem_(ItemID item, ItemRestType type);
	void DropSecondary();
	void DropPrimary();

	void OnSpawnEquip(bool addDefault = true, bool equipGame = true);
	void OnSpawnEquip_(bool addDefault = true, bool equipGame = true);

	void RemoveBomb();
	void HideTimer();
	bool MakeBomber();
	bool MakeBomber_();

	bool GetIntoGame();
	bool GetIntoGame_();

	CBasePlayerItem *GetItemByName(const char *itemName);
	CBasePlayerItem *GetItemById(WeaponIdType weaponID);

#ifdef REGAMEDLL_API
	CCSPlayer *CSPlayer() const;
#endif

	// templates
	template<typename Functor>
	CBasePlayerItem *ForEachItem(int slot, const Functor &func)
	{
		auto item = m_rgpPlayerItems[ slot ];
		while (item)
		{
			if (func(item))
				return item;

			item = item->m_pNext;
		}
		return nullptr;
	}
	template<typename Functor>
	CBasePlayerItem *ForEachItem(const Functor &func)
	{
		for (auto item : m_rgpPlayerItems)
		{
			while (item)
			{
				if (func(item))
					return item;

				item = item->m_pNext;
			}
		}
		return nullptr;
	}
public:
	enum { MaxLocationLen = 32 };

	int random_seed;
	unsigned short m_usPlayerBleed;
	EHANDLE m_hObserverTarget;
	float m_flNextObserverInput;
	int m_iObserverWeapon;
	int m_iObserverC4State;
	bool m_bObserverHasDefuser;
	int m_iObserverLastMode;
	float m_flFlinchTime;
	float m_flAnimTime;
	bool m_bHighDamage;
	float m_flVelocityModifier;
	int m_iLastZoom;
	bool m_bResumeZoom;
	float m_flEjectBrass;
	ArmorType m_iKevlar;
	bool m_bNotKilled;
	TeamName m_iTeam;
	int m_iAccount;
	bool m_bHasPrimary;
	float m_flDeathThrowTime;
	int m_iThrowDirection;
	float m_flLastTalk;
	bool m_bJustConnected;
	bool m_bContextHelp;
	JoinState m_iJoiningState;
	CBaseEntity *m_pIntroCamera;
	float m_fIntroCamTime;
	float m_fLastMovement;
	bool m_bMissionBriefing;
	bool m_bTeamChanged;
	ModelName m_iModelName;
	int m_iTeamKills;
	int m_iIgnoreGlobalChat;
	bool m_bHasNightVision;
	bool m_bNightVisionOn;
	Vector m_vRecentPath[MAX_RECENT_PATH];
	float m_flIdleCheckTime;
	float m_flRadioTime;
	int m_iRadioMessages;
	bool m_bIgnoreRadio;
	bool m_bHasC4;
	bool m_bHasDefuser;
	bool m_bKilledByBomb;
	Vector m_vBlastVector;
	bool m_bKilledByGrenade;
	CHintMessageQueue m_hintMessageQueue;
	int m_flDisplayHistory;
	_Menu m_iMenu;
	int m_iChaseTarget;
	CBaseEntity *m_pChaseTarget;
	float m_fCamSwitch;
	bool m_bEscaped;
	bool m_bIsVIP;
	float m_tmNextRadarUpdate;
	Vector m_vLastOrigin;
	int m_iCurrentKickVote;
	float m_flNextVoteTime;
	bool m_bJustKilledTeammate;
	int m_iHostagesKilled;
	int m_iMapVote;
	bool m_bCanShoot;
	float m_flLastFired;
	float m_flLastAttackedTeammate;
	bool m_bHeadshotKilled;
	bool m_bPunishedForTK;
	bool m_bReceivesNoMoneyNextRound;
	int m_iTimeCheckAllowed;
	bool m_bHasChangedName;
	char m_szNewName[MAX_PLAYER_NAME_LENGTH];
	bool m_bIsDefusing;
	float m_tmHandleSignals;
	CUnifiedSignals m_signals;
	edict_t *m_pentCurBombTarget;
	int m_iPlayerSound;
	int m_iTargetVolume;
	int m_iWeaponVolume;
	int m_iExtraSoundTypes;
	int m_iWeaponFlash;
	float m_flStopExtraSoundTime;
	float m_flFlashLightTime;
	int m_iFlashBattery;
	int m_afButtonLast;
	int m_afButtonPressed;
	int m_afButtonReleased;
	edict_t *m_pentSndLast;
	float m_flSndRoomtype;
	float m_flSndRange;
	float m_flFallVelocity;
	int m_rgItems[MAX_ITEMS];
	int m_fNewAmmo;
	unsigned int m_afPhysicsFlags;
	float m_fNextSuicideTime;
	float m_flTimeStepSound;
	float m_flTimeWeaponIdle;
	float m_flSwimTime;
	float m_flDuckTime;
	float m_flWallJumpTime;
	float m_flSuitUpdate;
	int m_rgSuitPlayList[CSUITPLAYLIST];
	int m_iSuitPlayNext;
	int m_rgiSuitNoRepeat[CSUITNOREPEAT];
	float m_rgflSuitNoRepeatTime[CSUITNOREPEAT];
	int m_lastDamageAmount;
	float m_tbdPrev;
	float m_flgeigerRange;
	float m_flgeigerDelay;
	int m_igeigerRangePrev;
	int m_iStepLeft;
	char m_szTextureName[CBTEXTURENAMEMAX];
	char m_chTextureType;
	int m_idrowndmg;
	int m_idrownrestored;
	int m_bitsHUDDamage;
	BOOL m_fInitHUD;
	BOOL m_fGameHUDInitialized;
	int m_iTrain;
	BOOL m_fWeapon;
	EHANDLE m_pTank;
	float m_fDeadTime;
	BOOL m_fNoPlayerSound;
	BOOL m_fLongJump;
	float m_tSneaking;
	int m_iUpdateTime;
	int m_iClientHealth;
	int m_iClientBattery;
	int m_iHideHUD;
	int m_iClientHideHUD;
	int m_iFOV;
	int m_iClientFOV;
	int m_iNumSpawns;
	CBaseEntity *m_pObserver;
	CBasePlayerItem *m_rgpPlayerItems[MAX_ITEM_TYPES];
	CBasePlayerItem *m_pActiveItem;
	CBasePlayerItem *m_pClientActiveItem;
	CBasePlayerItem *m_pLastItem;
	int m_rgAmmo[MAX_AMMO_SLOTS];
	int m_rgAmmoLast[MAX_AMMO_SLOTS];
	Vector m_vecAutoAim;
	BOOL m_fOnTarget;
	int m_iDeaths;
	int m_izSBarState[SBAR_END];
	float m_flNextSBarUpdateTime;
	float m_flStatusBarDisappearDelay;
	char m_SbarString0[SBAR_STRING_SIZE];
	int m_lastx;
	int m_lasty;
	int m_nCustomSprayFrames;
	float m_flNextDecalTime;
	char m_szTeamName[TEAM_NAME_LENGTH];

	static TYPEDESCRIPTION IMPL(m_playerSaveData)[40];

/*protected:*/
	int m_modelIndexPlayer;
	char m_szAnimExtention[32];
	int m_iGaitsequence;

	float m_flGaitframe;
	float m_flGaityaw;
	Vector m_prevgaitorigin;
	float m_flPitch;
	float m_flYaw;
	float m_flGaitMovement;
	int m_iAutoWepSwitch;
	bool m_bVGUIMenus;
	bool m_bShowHints;
	bool m_bShieldDrawn;
	bool m_bOwnsShield;
	bool m_bWasFollowing;
	float m_flNextFollowTime;
	float m_flYawModifier;
	float m_blindUntilTime;
	float m_blindStartTime;
	float m_blindHoldTime;
	float m_blindFadeTime;
	int m_blindAlpha;
	float m_allowAutoFollowTime;
	char m_autoBuyString[MAX_AUTOBUY_LENGTH];
	char *m_rebuyString;
	RebuyStruct m_rebuyStruct;
	bool m_bIsInRebuy;
	float m_flLastUpdateTime;
	char m_lastLocation[MaxLocationLen];
	float m_progressStart;
	float m_progressEnd;
	bool m_bObserverAutoDirector;
	bool m_canSwitchObserverModes;
	float m_heartBeatTime;
	float m_intenseTimestamp;
	float m_silentTimestamp;
	MusicState m_musicState;
	float m_flLastCommandTime[COMMANDS_TO_TRACK];
};

class CWShield: public CBaseEntity
{
public:
	virtual void Spawn();
	virtual void EXPORT Touch(CBaseEntity *pOther);

#ifdef HOOK_GAMEDLL

	void Spawn_();
	void Touch_(CBaseEntity *pOther);

#endif

public:
	void SetCantBePickedUpByUser(CBaseEntity *pEntity, float time)
	{
		m_hEntToIgnoreTouchesFrom = pEntity;
		m_flTimeToIgnoreTouches = gpGlobals->time + time;
	}

public:
	EHANDLE m_hEntToIgnoreTouchesFrom;
	float m_flTimeToIgnoreTouches;
};

inline bool CBasePlayer::IsReloading() const
{
	CBasePlayerWeapon *weapon = static_cast<CBasePlayerWeapon *>(m_pActiveItem);
	if (weapon && weapon->m_fInReload)
		return true;

	return false;
}

#ifdef REGAMEDLL_API
inline CCSPlayer *CBasePlayer::CSPlayer() const {
	return reinterpret_cast<CCSPlayer *>(this->m_pEntity);
}
#endif

#ifdef REGAMEDLL_FIXES

// returns a CBaseEntity pointer to a player by index.  Only returns if the player is spawned and connected otherwise returns NULL
// Index is 1 based
inline CBasePlayer *UTIL_PlayerByIndex(int playerIndex)
{
	return (CBasePlayer *)GET_PRIVATE(INDEXENT(playerIndex));
}

#endif

inline CBasePlayer *UTIL_PlayerByIndexSafe(int playerIndex)
{
	CBasePlayer *player = nullptr;
	if (likely(playerIndex > 0 && playerIndex <= gpGlobals->maxClients))
		player = UTIL_PlayerByIndex(playerIndex);

	return player;
}

extern int gEvilImpulse101;
extern entvars_t *g_pevLastInflictor;
extern CBaseEntity *g_pLastSpawn;
extern CBaseEntity *g_pLastCTSpawn;
extern CBaseEntity *g_pLastTerroristSpawn;
extern BOOL gInitHUD;
extern cvar_t *sv_aim;

extern int giPrecacheGrunt;
extern int gmsgWeapPickup;
extern int gmsgHudText;
extern int gmsgHudTextArgs;
extern int gmsgShake;
extern int gmsgFade;
extern int gmsgFlashlight;
extern int gmsgFlashBattery;
extern int gmsgResetHUD;
extern int gmsgInitHUD;
extern int gmsgViewMode;
extern int gmsgShowGameTitle;
extern int gmsgCurWeapon;
extern int gmsgHealth;
extern int gmsgDamage;
extern int gmsgBattery;
extern int gmsgTrain;
extern int gmsgLogo;
extern int gmsgWeaponList;
extern int gmsgAmmoX;
extern int gmsgDeathMsg;
extern int gmsgScoreAttrib;
extern int gmsgScoreInfo;
extern int gmsgTeamInfo;
extern int gmsgTeamScore;
extern int gmsgGameMode;
extern int gmsgMOTD;
extern int gmsgServerName;
extern int gmsgAmmoPickup;
extern int gmsgItemPickup;
extern int gmsgHideWeapon;
extern int gmsgSayText;
extern int gmsgTextMsg;
extern int gmsgSetFOV;
extern int gmsgShowMenu;
extern int gmsgSendAudio;
extern int gmsgRoundTime;
extern int gmsgMoney;
extern int gmsgBlinkAcct;
extern int gmsgArmorType;
extern int gmsgStatusValue;
extern int gmsgStatusText;
extern int gmsgStatusIcon;
extern int gmsgBarTime;
extern int gmsgReloadSound;
extern int gmsgCrosshair;
extern int gmsgNVGToggle;
extern int gmsgRadar;
extern int gmsgSpectator;
extern int gmsgVGUIMenu;
extern int gmsgCZCareer;
extern int gmsgCZCareerHUD;
extern int gmsgTaskTime;
extern int gmsgTutorText;
extern int gmsgTutorLine;
extern int gmsgShadowIdx;
extern int gmsgTutorState;
extern int gmsgTutorClose;
extern int gmsgAllowSpec;
extern int gmsgBombDrop;
extern int gmsgBombPickup;
extern int gmsgHostagePos;
extern int gmsgHostageK;
extern int gmsgGeigerRange;
extern int gmsgSendCorpse;
extern int gmsgHLTV;
extern int gmsgSpecHealth;
extern int gmsgForceCam;
extern int gmsgADStop;
extern int gmsgReceiveW;
extern int gmsgScenarioIcon;
extern int gmsgBotVoice;
extern int gmsgBuyClose;
extern int gmsgItemStatus;
extern int gmsgLocation;
extern int gmsgSpecHealth2;
extern int gmsgBarTime2;
extern int gmsgBotProgress;
extern int gmsgBrass;
extern int gmsgFog;
extern int gmsgShowTimer;

void OLD_CheckBuyZone(CBasePlayer *player);
void OLD_CheckBombTarget(CBasePlayer *player);
void OLD_CheckRescueZone(CBasePlayer *player);

void BuyZoneIcon_Set(CBasePlayer *player);
void BuyZoneIcon_Clear(CBasePlayer *player);
void BombTargetFlash_Set(CBasePlayer *player);
void BombTargetFlash_Clear(CBasePlayer *player);
void RescueZoneIcon_Set(CBasePlayer *player);
void RescueZoneIcon_Clear(CBasePlayer *player);
void EscapeZoneIcon_Set(CBasePlayer *player);
void EscapeZoneIcon_Clear(CBasePlayer *player);
void EscapeZoneIcon_Set(CBasePlayer *player);
void EscapeZoneIcon_Clear(CBasePlayer *player);
void VIP_SafetyZoneIcon_Set(CBasePlayer *player);
void VIP_SafetyZoneIcon_Clear(CBasePlayer *player);

void LinkUserMessages();
void WriteSigonMessages();
void SendItemStatus(CBasePlayer *pPlayer);
const char *GetCSModelName(int item_id);
Vector VecVelocityForDamage(float flDamage);
int TrainSpeed(int iSpeed, int iMax);
const char *GetWeaponName(entvars_t *pevInflictor, entvars_t *pKiller);
void LogAttack(CBasePlayer *pAttacker, CBasePlayer *pVictim, int teamAttack, int healthHit, int armorHit, int newHealth, int newArmor, const char *killer_weapon_name);
void packPlayerItem(CBasePlayer *pPlayer, CBasePlayerItem *pItem, bool packAmmo);
bool CanSeeUseable(CBasePlayer *me, CBaseEntity *entity);
void FixPlayerCrouchStuck(edict_t *pPlayer);
BOOL IsSpawnPointValid(CBaseEntity *pPlayer, CBaseEntity *pSpot);
void InitZombieSpawns();
CBaseEntity *FindZombieSpawn(CBaseEntity *player, bool forceSpawn);
CBaseEntity *FindEntityForward(CBaseEntity *pMe);
float_precision GetPlayerPitch(const edict_t *pEdict);
float_precision GetPlayerYaw(const edict_t *pEdict);
int GetPlayerGaitsequence(const edict_t *pEdict);
const char *GetBuyStringForWeaponClass(int weaponClass);
bool IsPrimaryWeaponClass(int classId);
bool IsPrimaryWeaponId(int id);
bool IsSecondaryWeaponClass(int classId);
bool IsSecondaryWeaponId(int id);
const char *GetWeaponAliasFromName(const char *weaponName);
bool CurrentWeaponSatisfies(CBasePlayerWeapon *pWeapon, int id, int classId);

#endif // PLAYER_H
