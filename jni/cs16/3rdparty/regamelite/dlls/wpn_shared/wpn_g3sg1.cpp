// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

LINK_ENTITY_TO_CLASS(weapon_g3sg1, CG3SG1, CCSG3SG1)

void CG3SG1::__MAKE_VHOOK(Spawn)()
{
	Precache();

	m_iId = WEAPON_G3SG1;
	SET_MODEL(edict(), "models/w_g3sg1.mdl");

	m_iDefaultAmmo = G3SG1_DEFAULT_GIVE;
	m_flLastFire = 0;

	FallInit();
}

void CG3SG1::__MAKE_VHOOK(Precache)()
{
	PRECACHE_MODEL("models/v_g3sg1.mdl");
	PRECACHE_MODEL("models/w_g3sg1.mdl");

	PRECACHE_SOUND("weapons/g3sg1-1.wav");
	PRECACHE_SOUND("weapons/g3sg1_slide.wav");
	PRECACHE_SOUND("weapons/g3sg1_clipin.wav");
	PRECACHE_SOUND("weapons/g3sg1_clipout.wav");
	PRECACHE_SOUND("weapons/zoom.wav");

	m_iShellId = m_iShell = PRECACHE_MODEL("models/rshell.mdl");
	m_usFireG3SG1 = PRECACHE_EVENT(1, "events/g3sg1.sc");
}

int CG3SG1::__MAKE_VHOOK(GetItemInfo)(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "762Nato";
	p->iMaxAmmo1 = MAX_AMMO_762NATO;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = G3SG1_MAX_CLIP;
	p->iSlot = 0;
	p->iPosition = 3;
	p->iId = m_iId = WEAPON_G3SG1;
	p->iFlags = 0;
	p->iWeight = G3SG1_WEIGHT;

	return 1;
}

BOOL CG3SG1::__MAKE_VHOOK(Deploy)()
{
	m_flAccuracy = 0.2f;
	return DefaultDeploy("models/v_g3sg1.mdl", "models/p_g3sg1.mdl", G3SG1_DRAW, "mp5", UseDecrement() != FALSE);
}

void CG3SG1::__MAKE_VHOOK(SecondaryAttack)()
{
	switch (m_pPlayer->m_iFOV)
	{
	case 90: m_pPlayer->m_iFOV = m_pPlayer->pev->fov = 40; break;
	case 40: m_pPlayer->m_iFOV = m_pPlayer->pev->fov = 15; break;
#ifdef REGAMEDLL_FIXES
	default:
#else
	case 15:
#endif
		m_pPlayer->m_iFOV = m_pPlayer->pev->fov = 90; break;
	}

	m_pPlayer->ResetMaxSpeed();

	if (TheBots != NULL)
	{
		TheBots->OnEvent(EVENT_WEAPON_ZOOMED, m_pPlayer);
	}

	EMIT_SOUND(m_pPlayer->edict(), CHAN_ITEM, "weapons/zoom.wav", 0.2, 2.4);
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.3f;
}

void CG3SG1::__MAKE_VHOOK(PrimaryAttack)()
{
	if (!(m_pPlayer->pev->flags & FL_ONGROUND))
	{
		G3SG1Fire(0.45, 0.25, FALSE);
	}
	else if (m_pPlayer->pev->velocity.Length2D() > 0)
	{
		G3SG1Fire(0.15, 0.25, FALSE);
	}
	else if (m_pPlayer->pev->flags & FL_DUCKING)
	{
		G3SG1Fire(0.035, 0.25, FALSE);
	}
	else
	{
		G3SG1Fire(0.055, 0.25, FALSE);
	}
}

void CG3SG1::G3SG1Fire(float flSpread, float flCycleTime, BOOL fUseAutoAim)
{
	Vector vecAiming, vecSrc, vecDir;
	int flag;

	if (m_pPlayer->pev->fov == DEFAULT_FOV)
	{
		flSpread += 0.025f;
	}

	if (m_flLastFire)
	{
		m_flAccuracy = (gpGlobals->time - m_flLastFire) * 0.3f + 0.55f;

		if (m_flAccuracy > 0.98f)
		{
			m_flAccuracy = 0.98f;
		}
	}
	else
	{
		m_flAccuracy = 0.98f;
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

	--m_iClip;
	m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);

	m_pPlayer->m_iWeaponVolume = BIG_EXPLOSION_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	vecSrc = m_pPlayer->GetGunPosition();
	vecAiming = gpGlobals->v_forward;

	vecDir = m_pPlayer->FireBullets3(vecSrc, vecAiming, (1 - m_flAccuracy) * flSpread, 8192, 3, BULLET_PLAYER_762MM, G3SG1_DAMAGE, G3SG1_RANGE_MODIFER, m_pPlayer->pev, true, m_pPlayer->random_seed);

#ifdef CLIENT_WEAPONS
	flag = FEV_NOTHOST;
#else
	flag = 0;
#endif

	PLAYBACK_EVENT_FULL(flag, m_pPlayer->edict(), m_usFireG3SG1, 0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y,
		int(m_pPlayer->pev->punchangle.x * 100), int(m_pPlayer->pev->punchangle.x * 100), FALSE, FALSE);

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = GetNextAttackDelay(flCycleTime);

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
	{
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.8f;

	m_pPlayer->pev->punchangle.x -= UTIL_SharedRandomFloat(m_pPlayer->random_seed + 4, 0.75, 1.75) + m_pPlayer->pev->punchangle.x * 0.25f;
	m_pPlayer->pev->punchangle.y += UTIL_SharedRandomFloat(m_pPlayer->random_seed + 5, -0.75, 0.75);
}

void CG3SG1::__MAKE_VHOOK(Reload)()
{
	if (m_pPlayer->ammo_762nato <= 0)
		return;

	if (DefaultReload(iMaxClip(), G3SG1_RELOAD, G3SG1_RELOAD_TIME))
	{
		m_flAccuracy = 0.2f;
		m_pPlayer->SetAnimation(PLAYER_RELOAD);

		if (m_pPlayer->pev->fov != DEFAULT_FOV)
		{
			m_pPlayer->m_iFOV = m_pPlayer->pev->fov = 15;
			SecondaryAttack();
		}
	}
}

void CG3SG1::__MAKE_VHOOK(WeaponIdle)()
{
	ResetEmptySound();
	m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle <= UTIL_WeaponTimeBase())
	{
		if (m_iClip)
		{
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 60.0f;
			SendWeaponAnim(G3SG1_IDLE, UseDecrement() != FALSE);
		}
	}
}

float CG3SG1::__MAKE_VHOOK(GetMaxSpeed)()
{
	return (m_pPlayer->m_iFOV == DEFAULT_FOV) ? G3SG1_MAX_SPEED : G3SG1_MAX_SPEED_ZOOM;
}
