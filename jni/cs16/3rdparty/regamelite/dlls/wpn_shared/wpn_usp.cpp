// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

LINK_ENTITY_TO_CLASS(weapon_usp, CUSP, CCSUSP)

void CUSP::__MAKE_VHOOK(Spawn)()
{
	Precache();

	m_iId = WEAPON_USP;
	SET_MODEL(ENT(pev), "models/w_usp.mdl");

	m_iWeaponState &= ~WPNSTATE_SHIELD_DRAWN;
	m_iDefaultAmmo = USP_DEFAULT_GIVE;
	m_flAccuracy = 0.92f;

	FallInit();
}

void CUSP::__MAKE_VHOOK(Precache)()
{
	PRECACHE_MODEL("models/v_usp.mdl");
	PRECACHE_MODEL("models/w_usp.mdl");
	PRECACHE_MODEL("models/shield/v_shield_usp.mdl");

	PRECACHE_SOUND("weapons/usp1.wav");
	PRECACHE_SOUND("weapons/usp2.wav");
	PRECACHE_SOUND("weapons/usp_unsil-1.wav");
	PRECACHE_SOUND("weapons/usp_clipout.wav");
	PRECACHE_SOUND("weapons/usp_clipin.wav");
	PRECACHE_SOUND("weapons/usp_silencer_on.wav");
	PRECACHE_SOUND("weapons/usp_silencer_off.wav");
	PRECACHE_SOUND("weapons/usp_sliderelease.wav");
	PRECACHE_SOUND("weapons/usp_slideback.wav");

	m_iShell = PRECACHE_MODEL("models/pshell.mdl");
	m_usFireUSP = PRECACHE_EVENT(1, "events/usp.sc");
}

int CUSP::__MAKE_VHOOK(GetItemInfo)(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
#ifdef REGAMEDLL_FIXES
	p->pszAmmo1 = "45acp";
#else
	p->pszAmmo1 = "45ACP";
#endif // REGAMEDLL_FIXES
	p->iMaxAmmo1 = MAX_AMMO_45ACP;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = USP_MAX_CLIP;
	p->iSlot = 1;
	p->iPosition = 4;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_USP;
	p->iWeight = USP_WEIGHT;

	return 1;
}

BOOL CUSP::__MAKE_VHOOK(Deploy)()
{
	m_iWeaponState &= ~WPNSTATE_SHIELD_DRAWN;
	m_flAccuracy = 0.92f;
	m_fMaxSpeed = USP_MAX_SPEED;
	m_pPlayer->m_bShieldDrawn = false;

	if (m_pPlayer->HasShield())
	{
		m_iWeaponState &= ~WPNSTATE_USP_SILENCED;
		return DefaultDeploy("models/shield/v_shield_usp.mdl", "models/shield/p_shield_usp.mdl", USP_SHIELD_DRAW, "shieldgun", UseDecrement());
	}
	else if (m_iWeaponState & WPNSTATE_USP_SILENCED)
	{
		return DefaultDeploy("models/v_usp.mdl", "models/p_usp.mdl", USP_DRAW, "onehanded", UseDecrement());
	}

	return DefaultDeploy("models/v_usp.mdl", "models/p_usp.mdl", USP_UNSIL_DRAW, "onehanded", UseDecrement());
}

void CUSP::__MAKE_VHOOK(SecondaryAttack)()
{
	if (ShieldSecondaryFire(USP_SHIELD_UP, USP_SHIELD_DOWN))
	{
		return;
	}

	if (m_iWeaponState & WPNSTATE_USP_SILENCED)
	{
		m_iWeaponState &= ~WPNSTATE_USP_SILENCED;

		SendWeaponAnim(USP_DETACH_SILENCER, UseDecrement() != FALSE);
		Q_strcpy(m_pPlayer->m_szAnimExtention, "onehanded");
	}
	else
	{
		m_iWeaponState |= WPNSTATE_USP_SILENCED;

		SendWeaponAnim(USP_ATTACH_SILENCER, UseDecrement() != FALSE);
		Q_strcpy(m_pPlayer->m_szAnimExtention, "onehanded");
	}

	m_flNextSecondaryAttack = m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3.0f;
	m_flNextPrimaryAttack = GetNextAttackDelay(3.0);
}

void CUSP::__MAKE_VHOOK(PrimaryAttack)()
{
	if (m_iWeaponState & WPNSTATE_USP_SILENCED)
	{
		if (!(m_pPlayer->pev->flags & FL_ONGROUND))
		{
			USPFire(1.3 * (1 - m_flAccuracy), 0.225, FALSE);
		}
		else if (m_pPlayer->pev->velocity.Length2D() > 0)
		{
			USPFire(0.25 * (1 - m_flAccuracy), 0.225, FALSE);
		}
		else if (m_pPlayer->pev->flags & FL_DUCKING)
		{
			USPFire(0.125 * (1 - m_flAccuracy), 0.225, FALSE);
		}
		else
		{
			USPFire(0.15 * (1 - m_flAccuracy), 0.225, FALSE);
		}
	}
	else
	{
		if (!(m_pPlayer->pev->flags & FL_ONGROUND))
		{
			USPFire(1.2 * (1 - m_flAccuracy), 0.225, FALSE);
		}
		else if (m_pPlayer->pev->velocity.Length2D() > 0)
		{
			USPFire(0.225 * (1 - m_flAccuracy), 0.225, FALSE);
		}
		else if (m_pPlayer->pev->flags & FL_DUCKING)
		{
			USPFire(0.08 * (1 - m_flAccuracy), 0.225, FALSE);
		}
		else
		{
			USPFire(0.1 * (1 - m_flAccuracy), 0.225, FALSE);
		}
	}
}

void CUSP::USPFire(float flSpread, float flCycleTime, BOOL fUseSemi)
{
	int flag;
	int iDamage;
	Vector vecAiming, vecSrc, vecDir;

	flCycleTime -= 0.075f;

	if (++m_iShotsFired > 1)
	{
		return;
	}

	if (m_flLastFire != 0.0f)
	{
		m_flAccuracy -= (0.3f - (gpGlobals->time - m_flLastFire)) * 0.275f;

		if (m_flAccuracy > 0.92f)
		{
			m_flAccuracy = 0.92f;
		}
		else if (m_flAccuracy < 0.6f)
		{
			m_flAccuracy = 0.6f;
		}
	}

	m_flLastFire = gpGlobals->time;

	if (m_iClip <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = GetNextAttackDelay(0.2);
		}

		if (TheBots != NULL)
		{
			TheBots->OnEvent(EVENT_WEAPON_FIRED_ON_EMPTY, m_pPlayer);
		}

		return;
	}

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = GetNextAttackDelay(flCycleTime);

	--m_iClip;
	SetPlayerShieldAnim();

	m_pPlayer->SetAnimation(PLAYER_ATTACK1);
	m_pPlayer->m_iWeaponVolume = BIG_EXPLOSION_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);

	if (!(m_iWeaponState & WPNSTATE_USP_SILENCED))
	{
		m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
	}

	vecSrc = m_pPlayer->GetGunPosition();
	vecAiming = gpGlobals->v_forward;

	iDamage = (m_iWeaponState & WPNSTATE_USP_SILENCED) ? USP_DAMAGE_SIL : USP_DAMAGE;

	vecDir = m_pPlayer->FireBullets3(vecSrc, vecAiming, flSpread, 4096, 1, BULLET_PLAYER_45ACP, iDamage, USP_RANGE_MODIFER, m_pPlayer->pev, true, m_pPlayer->random_seed);

#ifdef CLIENT_WEAPONS
	flag = FEV_NOTHOST;
#else
	flag = 0;
#endif // CLIENT_WEAPONS

	PLAYBACK_EVENT_FULL(flag, m_pPlayer->edict(), m_usFireUSP, 0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y,
		(int)(m_pPlayer->pev->punchangle.x * 100), 0, m_iClip == 0, (m_iWeaponState & WPNSTATE_USP_SILENCED));

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
	{
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, FALSE);
	}
	
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0f;
	m_pPlayer->pev->punchangle.x -= 2.0f;
	ResetPlayerShieldAnim();
}

void CUSP::__MAKE_VHOOK(Reload)()
{
	if (m_pPlayer->ammo_45acp <= 0)
		return;

	int iAnim;
	if (m_pPlayer->HasShield())
		iAnim = USP_SHIELD_RELOAD;
	else if (m_iWeaponState & WPNSTATE_USP_SILENCED)
		iAnim = USP_RELOAD;
	else
		iAnim = USP_UNSIL_RELOAD;

	if (DefaultReload(iMaxClip(), iAnim, USP_RELOAD_TIME))
	{
		m_pPlayer->SetAnimation(PLAYER_RELOAD);
		m_flAccuracy = 0.92f;
	}
}

void CUSP::__MAKE_VHOOK(WeaponIdle)()
{
	ResetEmptySound();
	m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);
	
	if (m_flTimeWeaponIdle > 0)
	{
		return;
	}

	if (m_pPlayer->HasShield())
	{
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 20.0f;

		if (m_iWeaponState & WPNSTATE_SHIELD_DRAWN)
		{
			SendWeaponAnim(USP_DRAW, UseDecrement());
		}
	}
	else if (m_iClip)
	{
		int iAnim = (~m_iWeaponState & WPNSTATE_USP_SILENCED) ? USP_UNSIL_IDLE: USP_IDLE;

		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 60.0f;
		SendWeaponAnim(iAnim, UseDecrement());
	}
}
