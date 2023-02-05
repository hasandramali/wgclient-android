// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

/*
* Globals initialization
*/
#ifndef HOOK_GAMEDLL

TYPEDESCRIPTION CGrenade::m_SaveData[] =
{
	DEFINE_FIELD(CGrenade, m_fAttenu, FIELD_FLOAT),
	DEFINE_FIELD(CGrenade, m_flNextFreq, FIELD_TIME),
	DEFINE_FIELD(CGrenade, m_flC4Blow, FIELD_TIME),
	DEFINE_FIELD(CGrenade, m_flNextFreqInterval, FIELD_TIME),
	DEFINE_FIELD(CGrenade, m_flNextBeep, FIELD_TIME),
	DEFINE_FIELD(CGrenade, m_flDefuseCountDown, FIELD_TIME),
	DEFINE_FIELD(CGrenade, m_flNextBlink, FIELD_TIME),
	DEFINE_FIELD(CGrenade, m_pentCurBombTarget, FIELD_EDICT),
	DEFINE_FIELD(CGrenade, m_sBeepName, FIELD_POINTER),
	DEFINE_FIELD(CGrenade, m_bIsC4, FIELD_BOOLEAN),
	DEFINE_FIELD(CGrenade, m_bStartDefuse, FIELD_BOOLEAN),
	DEFINE_FIELD(CGrenade, m_SGSmoke, FIELD_INTEGER),
	DEFINE_FIELD(CGrenade, m_bJustBlew, FIELD_BOOLEAN),
	DEFINE_FIELD(CGrenade, m_bLightSmoke, FIELD_BOOLEAN),
	DEFINE_FIELD(CGrenade, m_usEvent, FIELD_INTEGER),
};

#endif

LINK_ENTITY_TO_CLASS(grenade, CGrenade, CCSGrenade)

void CGrenade::Explode(Vector vecSrc, Vector vecAim)
{
	TraceResult tr;
	UTIL_TraceLine(pev->origin, pev->origin + Vector(0, 0, -32), ignore_monsters, ENT(pev), &tr);
	Explode(&tr, DMG_BLAST);
}

// UNDONE: temporary scorching for PreAlpha - find a less sleazy permenant solution.
void CGrenade::Explode(TraceResult *pTrace, int bitsDamageType)
{
	float flRndSound; // sound randomizer

	pev->model = iStringNull; // invisible
	pev->solid = SOLID_NOT; // intangible
	pev->takedamage = DAMAGE_NO;

	// Pull out of the wall a bit
	if (pTrace->flFraction != 1.0f)
	{
		pev->origin = pTrace->vecEndPos + (pTrace->vecPlaneNormal * (pev->dmg - 24.0f) * 0.6f);
	}

	int iContents = UTIL_PointContents(pev->origin);

#ifndef REGAMEDLL_FIXES
	CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3);
#endif
	entvars_t *pevOwner = VARS(pev->owner);

	if (TheBots)
	{
		TheBots->OnEvent(EVENT_FLASHBANG_GRENADE_EXPLODED, CBaseEntity::Instance(pev->owner), (CBaseEntity *)&pev->origin);
	}

	// can't traceline attack owner if this is set
	pev->owner = NULL;

	RadiusFlash(pev->origin, pev, pevOwner, 4, CLASS_NONE, bitsDamageType);

	if (RANDOM_FLOAT(0, 1) < 0.5f)
		UTIL_DecalTrace(pTrace, DECAL_SCORCH1);
	else
		UTIL_DecalTrace(pTrace, DECAL_SCORCH2);

	// TODO: unused
	flRndSound = RANDOM_FLOAT(0, 1);

	switch (RANDOM_LONG(0, 1))
	{
	case 0: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/flashbang-2.wav", 0.55, ATTN_NORM); break;
	case 1: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/flashbang-1.wav", 0.55, ATTN_NORM); break;
	}

	pev->effects |= EF_NODRAW;
	SetThink(&CGrenade::Smoke);
	pev->velocity = g_vecZero;
	pev->nextthink = gpGlobals->time + 0.3f;

	if (iContents != CONTENTS_WATER)
	{
		int sparkCount = RANDOM_LONG(0, 3);

		for (int i = 0; i < sparkCount; ++i)
			Create("spark_shower", pev->origin, pTrace->vecPlaneNormal, NULL);
	}
}

void CGrenade::Explode2(TraceResult *pTrace, int bitsDamageType)
{
	float flRndSound; // sound randomizer

	pev->model = iStringNull; // invisible
	pev->solid = SOLID_NOT; // intangible
	pev->takedamage = DAMAGE_NO;

	UTIL_ScreenShake(pTrace->vecEndPos, 25, 150, 1, 3000);

	CSGameRules()->m_bTargetBombed = true;

	if (CSGameRules()->IsCareer())
	{
		if (TheCareerTasks)
		{
			TheCareerTasks->LatchRoundEndMessage();
		}
	}

	m_bJustBlew = true;
	CSGameRules()->CheckWinConditions();

	// Pull out of the wall a bit
	if (pTrace->flFraction != 1.0f)
	{
		pev->origin = pTrace->vecEndPos + (pTrace->vecPlaneNormal * (pev->dmg - 24.0f) * 0.6f);
	}

	int iContents = UTIL_PointContents(pev->origin);

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_SPRITE);
		WRITE_COORD(pev->origin.x);
		WRITE_COORD(pev->origin.y);
		WRITE_COORD(pev->origin.z - 10.0f);
		WRITE_SHORT(g_sModelIndexFireball3);
		WRITE_BYTE((pev->dmg - 275.0f) * 0.6f);
		WRITE_BYTE(150);
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_SPRITE);
		WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-512, 512));
		WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-512, 512));
		WRITE_COORD(pev->origin.z + RANDOM_FLOAT(-10, 10));
		WRITE_SHORT(g_sModelIndexFireball2);
		WRITE_BYTE((pev->dmg - 275.0f) * 0.6f);
		WRITE_BYTE(150);
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_SPRITE);
		WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-512, 512));
		WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-512, 512));
		WRITE_COORD(pev->origin.z + RANDOM_FLOAT(-10, 10));
		WRITE_SHORT(g_sModelIndexFireball3);
		WRITE_BYTE((pev->dmg - 275.0f) * 0.6f);
		WRITE_BYTE(150);
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_SPRITE);
		WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-512, 512));
		WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-512, 512));
		WRITE_COORD(pev->origin.z + RANDOM_FLOAT(-10, 10));
		WRITE_SHORT(g_sModelIndexFireball);
		WRITE_BYTE((pev->dmg - 275.0f) * 0.6f);
		WRITE_BYTE(17);
	MESSAGE_END();

	// Sound! for everyone
	EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/c4_explode1.wav", VOL_NORM, 0.25);

#ifndef REGAMEDLL_FIXES
	CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3);
#endif
	entvars_t *pevOwner = VARS(pev->owner);

	pev->owner = NULL;
	RadiusDamage(pev, pevOwner, CSGameRules()->m_flBombRadius, CLASS_NONE, bitsDamageType);

	if (CSGameRules()->IsCareer())
	{
		if (TheCareerTasks)
		{
			TheCareerTasks->UnlatchRoundEndMessage();
		}
	}

	// tell director about it
	// send director message, that something important happed here
	MESSAGE_BEGIN(MSG_SPEC, SVC_DIRECTOR);
		WRITE_BYTE(9);		// command length in bytes
		WRITE_BYTE(DRC_CMD_EVENT);	// bomb explode
		WRITE_SHORT(ENTINDEX(edict()));	// index number of primary entity
		WRITE_SHORT(0);		// index number of secondary entity
		WRITE_LONG(15 | DRC_FLAG_FINAL);	// eventflags (priority and flags)
	MESSAGE_END();

	// Decal!
	if (RANDOM_FLOAT(0, 1) < 0.5f)
		UTIL_DecalTrace(pTrace, DECAL_SCORCH1);
	else
		UTIL_DecalTrace(pTrace, DECAL_SCORCH2);

	// TODO: unused
	flRndSound = RANDOM_FLOAT(0, 1);

	switch (RANDOM_LONG(0, 2))
	{
	case 0: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/debris1.wav", 0.55, ATTN_NORM); break;
	case 1: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/debris2.wav", 0.55, ATTN_NORM); break;
	case 2: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/debris3.wav", 0.55, ATTN_NORM); break;
	}

	pev->effects |= EF_NODRAW;
	SetThink(&CGrenade::Smoke2);
	pev->velocity = g_vecZero;
	pev->nextthink = gpGlobals->time + 0.85f;

	if (iContents != CONTENTS_WATER)
	{
		int sparkCount = RANDOM_LONG(0, 3);

		for (int i = 0; i < sparkCount; ++i)
			Create("spark_shower", pev->origin, pTrace->vecPlaneNormal, NULL);
	}
}

void CGrenade::Explode3(TraceResult *pTrace, int bitsDamageType)
{
	float flRndSound; // sound randomizer
	//float damage;

	pev->model = iStringNull; // invisible
	pev->solid = SOLID_NOT; // intangible
	pev->takedamage = DAMAGE_NO;

	if (pTrace->flFraction != 1.0f)
	{
		pev->origin = pTrace->vecEndPos + (pTrace->vecPlaneNormal * (pev->dmg - 24.0f) * 0.6f);
	}

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_EXPLOSION);	// This makes a dynamic light and the explosion sprites/sound
		WRITE_COORD(pev->origin.x);		// Send to PAS because of the sound
		WRITE_COORD(pev->origin.y);
		WRITE_COORD(pev->origin.z + 20.0f);
		WRITE_SHORT(g_sModelIndexFireball3);
		WRITE_BYTE(25);			// scale * 10
		WRITE_BYTE(30);		// framerate
		WRITE_BYTE(TE_EXPLFLAG_NONE);	// flags
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_EXPLOSION);	// This makes a dynamic light and the explosion sprites/sound
		WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-64, 64));	// Send to PAS because of the sound
		WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-64, 64));
		WRITE_COORD(pev->origin.z + RANDOM_FLOAT(30, 35));
		WRITE_SHORT(g_sModelIndexFireball2);
		WRITE_BYTE(30);			// scale * 10
		WRITE_BYTE(30);		// framerate
		WRITE_BYTE(TE_EXPLFLAG_NONE);	// flags
	MESSAGE_END();

#ifndef REGAMEDLL_FIXES
	CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3);
#endif
	entvars_t *pevOwner = VARS(pev->owner);

	if (TheBots)
	{
		TheBots->OnEvent(EVENT_HE_GRENADE_EXPLODED, CBaseEntity::Instance(pev->owner));
	}

	pev->owner = NULL;
	RadiusDamage(pev, pevOwner, pev->dmg, CLASS_NONE, bitsDamageType);

	if (RANDOM_FLOAT(0, 1) < 0.5f)
		UTIL_DecalTrace(pTrace, DECAL_SCORCH1);
	else
		UTIL_DecalTrace(pTrace, DECAL_SCORCH2);

	// TODO: unused
	flRndSound = RANDOM_FLOAT(0, 1);

	switch (RANDOM_LONG(0, 2))
	{
	case 0: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/debris1.wav", 0.55, ATTN_NORM); break;
	case 1: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/debris2.wav", 0.55, ATTN_NORM); break;
	case 2: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/debris3.wav", 0.55, ATTN_NORM); break;
	}

	pev->effects |= EF_NODRAW;
	SetThink(&CGrenade::Smoke3_C);
	pev->velocity = g_vecZero;
	pev->nextthink = gpGlobals->time + 0.55f;
	int sparkCount = RANDOM_LONG(0, 3);

	for (int i = 0; i < sparkCount; ++i)
		Create("spark_shower", pev->origin, pTrace->vecPlaneNormal, NULL);
}

NOXREF void CGrenade::SG_Explode(TraceResult *pTrace, int bitsDamageType)
{
	float flRndSound; // sound randomizer
	//entvars_t *pevOwner;

	pev->model = iStringNull; // invisible
	pev->solid = SOLID_NOT; // intangible

	pev->takedamage = DAMAGE_NO;

	if (pTrace->flFraction != 1.0f)
	{
		pev->origin = pTrace->vecEndPos + (pTrace->vecPlaneNormal * (pev->dmg - 24.0f) * 0.6f);
	}

	int iContents = UTIL_PointContents(pev->origin);

#ifndef REGAMEDLL_FIXES
	CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3);
#endif

	// can't traceline attack owner if this is set
	pev->owner = NULL;

	if (RANDOM_FLOAT(0, 1) < 0.5f)
		UTIL_DecalTrace(pTrace, DECAL_SCORCH1);
	else
		UTIL_DecalTrace(pTrace, DECAL_SCORCH2);

	// TODO: unused
	flRndSound = RANDOM_FLOAT(0, 1);

	switch (RANDOM_LONG(0, 1))
	{
	case 0: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/flashbang-2.wav", 0.55, ATTN_NORM); break;
	case 1: EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/flashbang-1.wav", 0.55, ATTN_NORM); break;
	}

	pev->effects |= EF_NODRAW;
	SetThink(&CGrenade::Smoke);
	pev->velocity = g_vecZero;
	pev->nextthink = gpGlobals->time + 0.1f;

	if (iContents != CONTENTS_WATER)
	{
		int sparkCount = RANDOM_LONG(0, 3);

		for (int i = 0; i < sparkCount; ++i)
			Create("spark_shower", pev->origin, pTrace->vecPlaneNormal, NULL);
	}
}

void CGrenade::Smoke3_C()
{
	if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
	{
		UTIL_Bubbles(pev->origin - Vector(64, 64, 64), pev->origin + Vector(64, 64, 64), 100);
	}
	else
	{
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_SMOKE);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z - 5.0f);
			WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(35 + RANDOM_FLOAT(0, 10)); // scale * 10
			WRITE_BYTE(5); // framerate
		MESSAGE_END();
	}

	UTIL_Remove(this);
}

void CGrenade::Smoke3_B()
{
	if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
	{
		UTIL_Bubbles(pev->origin - Vector(64, 64, 64), pev->origin + Vector(64, 64, 64), 100);
	}
	else
	{
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_SMOKE);
			WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-128, 128));
			WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-128, 128));
			WRITE_COORD(pev->origin.z + RANDOM_FLOAT(-10, 10));
			WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(15 + RANDOM_FLOAT(0, 10)); // scale * 10
			WRITE_BYTE(10); // framerate
		MESSAGE_END();
	}

	pev->nextthink = gpGlobals->time + 0.15f;
	SetThink(&CGrenade::Smoke3_A);
}

void CGrenade::Smoke3_A()
{
	if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
	{
		UTIL_Bubbles(pev->origin - Vector(64, 64, 64), pev->origin + Vector(64, 64, 64), 100);
	}
	else
	{
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_SMOKE);
			WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-128, 128));
			WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-128, 128));
			WRITE_COORD(pev->origin.z + RANDOM_FLOAT(-10, 10));
			WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(15 + RANDOM_FLOAT(0, 10)); // scale * 10
			WRITE_BYTE(12); // framerate
		MESSAGE_END();
	}
}

void CGrenade::Smoke2()
{
	if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
	{
		UTIL_Bubbles(pev->origin - Vector(64, 64, 64), pev->origin + Vector(64, 64, 64), 100);
	}
	else
	{
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_SMOKE);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z);
			WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(150); // scale * 10
			WRITE_BYTE(8); // framerate
		MESSAGE_END();
	}

	UTIL_Remove(this);
}

void CGrenade::Smoke()
{
	if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
	{
		UTIL_Bubbles(pev->origin - Vector(64, 64, 64), pev->origin + Vector(64, 64, 64), 100);
	}
	else
	{
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_SMOKE);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z);
			WRITE_SHORT(g_sModelIndexSmoke);
			WRITE_BYTE(25); // scale * 10
			WRITE_BYTE(6); // framerate
		MESSAGE_END();
	}

	UTIL_Remove(this);
}

void CGrenade::SG_Smoke()
{
	int iMaxSmokePuffs = 100;
	float flSmokeInterval;

	if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
	{
		UTIL_Bubbles(pev->origin - Vector(64, 64, 64), pev->origin + Vector(64, 64, 64), 100);
	}
	else
	{
		Vector origin, angle;
		float_precision x_old, y_old, R_angle;

		UTIL_MakeVectors(pev->angles);

		origin = gpGlobals->v_forward * RANDOM_FLOAT(3, 8);

		flSmokeInterval = RANDOM_FLOAT(1.5f, 3.5f) * iMaxSmokePuffs;

		R_angle = m_angle / (180.00433335 / M_PI);

		x_old = Q_cos(float_precision(R_angle));
		y_old = Q_sin(float_precision(R_angle));

		angle.x = origin.x * x_old - origin.y * y_old;
		angle.y = origin.x * y_old + origin.y * x_old;

		m_angle = (m_angle + 30) % 360;

		PLAYBACK_EVENT_FULL(0, NULL, m_usEvent, 0, pev->origin, m_vSmokeDetonate, angle.x, angle.y, flSmokeInterval, 4, m_bLightSmoke, 6);
	}

	if (m_SGSmoke <= 20)
	{
		pev->nextthink = gpGlobals->time + 1.0f;
		SetThink(&CGrenade::SG_Smoke);
		++m_SGSmoke;
	}
	else
	{
		pev->effects |= EF_NODRAW;

		if (TheBots)
		{
			TheBots->RemoveGrenade(this);
		}
		UTIL_Remove(this);
	}
}

void CGrenade::__MAKE_VHOOK(Killed)(entvars_t *pevAttacker, int iGib)
{
	Detonate();
}

// Timed grenade, this think is called when time runs out.
void CGrenade::DetonateUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	SetThink(&CGrenade::Detonate);
	pev->nextthink = gpGlobals->time;
}

void CGrenade::PreDetonate()
{
#ifndef REGAMEDLL_FIXES
	CSoundEnt::InsertSound(bits_SOUND_DANGER, pev->origin, 400, 0.3);
#endif

	SetThink(&CGrenade::Detonate);
	pev->nextthink = gpGlobals->time + 1.0f;
}

void CGrenade::Detonate()
{
	TraceResult tr;
	Vector vecSpot; // trace starts here!

	vecSpot = pev->origin + Vector(0, 0, 8);
	UTIL_TraceLine(vecSpot, vecSpot + Vector(0, 0, -40), ignore_monsters, ENT(pev), &tr);
	Explode(&tr, DMG_BLAST);
}

void CGrenade::SG_Detonate()
{
	TraceResult tr;
	Vector vecSpot;
	edict_t *pentFind = NULL;

	vecSpot = pev->origin + Vector(0, 0, 8);

	UTIL_TraceLine(vecSpot, vecSpot + Vector(0, 0, -40), ignore_monsters, ENT(pev), &tr);

	if (TheBots)
	{
		TheBots->OnEvent(EVENT_SMOKE_GRENADE_EXPLODED, CBaseEntity::Instance(pev->owner));
		TheBots->AddGrenade(WEAPON_SMOKEGRENADE, this);
	}

	EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/sg_explode.wav", VOL_NORM, ATTN_NORM);

	while ((pentFind = FIND_ENTITY_BY_CLASSNAME(pentFind, "grenade")))
	{
		if (FNullEnt(pentFind))
			break;

		CBaseEntity *pEnt = CBaseEntity::Instance(pentFind);
		if (pEnt)
		{
			float fDistance = (pEnt->pev->origin - pev->origin).Length();

			if (fDistance != 0.0f && fDistance <= 250.0f)
			{
				if (gpGlobals->time > pEnt->pev->dmgtime)
				{
					m_bLightSmoke = true;
				}
			}
		}
	}

	m_bDetonated = true;
	PLAYBACK_EVENT_FULL(0, NULL, m_usEvent, 0, pev->origin, (float *)&g_vecZero, 0, 0, 0, 1, m_bLightSmoke, FALSE);
	m_vSmokeDetonate = pev->origin;

	pev->velocity.x = RANDOM_FLOAT(-175, 175);
	pev->velocity.y = RANDOM_FLOAT(-175, 175);
	pev->velocity.z = RANDOM_FLOAT(250, 350);

	pev->nextthink = gpGlobals->time + 0.1f;
	SetThink(&CGrenade::SG_Smoke);
}

void CGrenade::Detonate2()
{
	TraceResult tr;
	Vector vecSpot;// trace starts here!

	vecSpot = pev->origin + Vector(0, 0, 8);

	UTIL_TraceLine(vecSpot, vecSpot + Vector(0, 0, -40), ignore_monsters, ENT(pev), &tr);
	Explode2(&tr, DMG_BLAST);
}

void CGrenade::Detonate3()
{
	TraceResult tr;
	Vector vecSpot;// trace starts here!

	vecSpot = pev->origin + Vector(0, 0, 8);
	UTIL_TraceLine(vecSpot, vecSpot + Vector(0, 0, -40), ignore_monsters, ENT(pev), &tr);
	Explode3(&tr, DMG_EXPLOSION);
}

// Contact grenade, explode when it touches something
void CGrenade::ExplodeTouch(CBaseEntity *pOther)
{
	TraceResult tr;
	Vector vecSpot; // trace starts here!

	pev->enemy = pOther->edict();

	vecSpot = pev->origin - pev->velocity.Normalize() * 32.0f;
	UTIL_TraceLine(vecSpot, vecSpot + pev->velocity.Normalize() * 64, ignore_monsters, ENT(pev), &tr);
	Explode(&tr, DMG_BLAST);
}

void CGrenade::DangerSoundThink()
{
	if (!IsInWorld())
	{
		UTIL_Remove(this);
		return;
	}

#ifndef REGAMEDLL_FIXES
	CSoundEnt::InsertSound(bits_SOUND_DANGER, pev->origin + pev->velocity * 0.5, pev->velocity.Length(), 0.2);
#endif
	pev->nextthink = gpGlobals->time + 0.2f;

	if (pev->waterlevel != 0)
	{
		pev->velocity = pev->velocity * 0.5f;
	}
}

void CGrenade::BounceTouch(CBaseEntity *pOther)
{
	// don't hit the guy that launched this grenade
	if (pOther->edict() == pev->owner)
		return;

	if (FClassnameIs(pOther->pev, "func_breakable") && pOther->pev->rendermode != kRenderNormal)
	{
		pev->velocity = pev->velocity * -2.0f;
		return;
	}

	Vector vecTestVelocity;

	// this is my heuristic for modulating the grenade velocity because grenades dropped purely vertical
	// or thrown very far tend to slow down too quickly for me to always catch just by testing velocity.
	// trimming the Z velocity a bit seems to help quite a bit.
	vecTestVelocity = pev->velocity;
	vecTestVelocity.z *= 0.7f;

#ifndef REGAMEDLL_FIXES
	if (!m_fRegisteredSound && vecTestVelocity.Length() <= 60.0f)
	{
		// grenade is moving really slow. It's probably very close to where it will ultimately stop moving.
		// go ahead and emit the danger sound.

		// register a radius louder than the explosion, so we make sure everyone gets out of the way
		CSoundEnt::InsertSound(bits_SOUND_DANGER, pev->origin, pev->dmg / 0.4f, 0.3);
		m_fRegisteredSound = TRUE;
	}
#endif
	if (pev->flags & FL_ONGROUND)
	{
		// add a bit of static friction
		pev->velocity = pev->velocity * 0.8f;
		pev->sequence = RANDOM_LONG(1, 1);	// TODO: what?
	}
	else
	{
		if (m_iBounceCount < 5)
		{
			// play bounce sound
			BounceSound();
		}

		if (m_iBounceCount >= 10)
		{
			pev->groundentity = ENT(0);
			pev->flags |= FL_ONGROUND;
			pev->velocity = g_vecZero;
		}

		++m_iBounceCount;
	}

	pev->framerate = pev->velocity.Length() / 200.0f;

	if (pev->framerate > 1)
	{
		pev->framerate = 1.0f;
	}
	else if (pev->framerate < 0.5f)
	{
		pev->framerate = 0.0f;
	}
}

void CGrenade::SlideTouch(CBaseEntity *pOther)
{
	// don't hit the guy that launched this grenade
	if (pOther->edict() == pev->owner)
		return;

	if (pev->flags & FL_ONGROUND)
	{
		// add a bit of static friction
		pev->velocity = pev->velocity * 0.95f;
	}
	else
	{
		BounceSound();
	}
}

void CGrenade::__MAKE_VHOOK(BounceSound)()
{
	if (pev->dmg > 50.0f)
	{
		EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/he_bounce-1.wav", 0.25, ATTN_NORM);
		return;
	}

	switch (RANDOM_LONG(0, 2))
	{
	case 0:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/grenade_hit1.wav", 0.25, ATTN_NORM); break;
	case 1:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/grenade_hit2.wav", 0.25, ATTN_NORM); break;
	case 2:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/grenade_hit3.wav", 0.25, ATTN_NORM); break;
	}
}

void CGrenade::TumbleThink()
{
	if (!IsInWorld())
	{
		UTIL_Remove(this);
		return;
	}

	StudioFrameAdvance();
	pev->nextthink = gpGlobals->time + 0.1f;

#ifndef REGAMEDLL_FIXES
	if (pev->dmgtime - 1 < gpGlobals->time)
	{
		CSoundEnt::InsertSound(bits_SOUND_DANGER, pev->origin + pev->velocity * (pev->dmgtime - gpGlobals->time), 400, 0.1);
	}
#endif

	if (pev->dmgtime <= gpGlobals->time)
	{
		if (pev->dmg <= 40.0f)
		{
			SetThink(&CGrenade::Detonate);
		}
		else
			SetThink(&CGrenade::Detonate3);
	}

	if (pev->waterlevel != 0)
	{
		pev->velocity = pev->velocity * 0.5f;
		pev->framerate = 0.2f;
	}
}

void CGrenade::SG_TumbleThink()
{
	if (!IsInWorld())
	{
		UTIL_Remove(this);
		return;
	}

	if (pev->flags & FL_ONGROUND)
	{
		pev->velocity = pev->velocity * 0.95f;
	}

	StudioFrameAdvance();
	pev->nextthink = gpGlobals->time + 0.1f;

#ifndef REGAMEDLL_FIXES
	if (pev->dmgtime - 1 < gpGlobals->time)
	{
		CSoundEnt::InsertSound(bits_SOUND_DANGER, pev->origin + pev->velocity * (pev->dmgtime - gpGlobals->time), 400, 0.1);
	}
#endif

	if (pev->dmgtime <= gpGlobals->time)
	{
		if (pev->flags & FL_ONGROUND)
		{
			SetThink(&CGrenade::SG_Detonate);
		}
	}

	if (pev->waterlevel != 0)
	{
		pev->velocity = pev->velocity * 0.5f;
		pev->framerate = 0.2f;
	}
}

void CGrenade::__MAKE_VHOOK(Spawn)()
{
	m_iBounceCount = 0;
	pev->movetype = MOVETYPE_BOUNCE;

	MAKE_STRING_CLASS("grenade", pev);

	m_bIsC4 = false;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "models/grenade.mdl");
	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));

	pev->dmg = 30.0f;
	m_fRegisteredSound = FALSE;
}

NOXREF CGrenade *CGrenade::ShootContact(entvars_t *pevOwner, Vector vecStart, Vector vecVelocity)
{
	CGrenade *pGrenade = GetClassPtr<CCSGrenade>((CGrenade *)NULL);
	pGrenade->Spawn();

	// contact grenades arc lower
	pGrenade->pev->gravity = 0.5f;	// lower gravity since grenade is aerodynamic and engine doesn't know it.

	UTIL_SetOrigin(pGrenade->pev, vecStart);
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = UTIL_VecToAngles(pGrenade->pev->velocity);
	pGrenade->pev->owner = ENT(pevOwner);

	// make monsters afaid of it while in the air
	pGrenade->SetThink(&CGrenade::DangerSoundThink);
	pGrenade->pev->nextthink = gpGlobals->time;

	// Tumble in air
	pGrenade->pev->avelocity.x = RANDOM_FLOAT(-100, -500);

	// Explode on contact
	pGrenade->SetTouch(&CGrenade::ExplodeTouch);

	pGrenade->pev->dmg = gSkillData.plrDmgM203Grenade;
	pGrenade->m_bJustBlew = true;
	return pGrenade;
}

CGrenade *CGrenade::ShootTimed2(entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time, int iTeam, unsigned short usEvent)
{
	CGrenade *pGrenade = GetClassPtr<CCSGrenade>((CGrenade *)NULL);
	pGrenade->Spawn();

	UTIL_SetOrigin(pGrenade->pev, vecStart);
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = pevOwner->angles;
	pGrenade->pev->owner = ENT(pevOwner);

	pGrenade->m_usEvent = usEvent;

	pGrenade->SetTouch(&CGrenade::BounceTouch);

	pGrenade->pev->dmgtime = gpGlobals->time + time;
	pGrenade->SetThink(&CGrenade::TumbleThink);
	pGrenade->pev->nextthink = gpGlobals->time + 0.1f;

	pGrenade->pev->sequence = RANDOM_LONG(3, 6);
	pGrenade->pev->framerate = 1.0f;

	pGrenade->m_bJustBlew = true;

	pGrenade->pev->gravity = 0.55f;
	pGrenade->pev->friction = 0.7f;

	pGrenade->m_iTeam = iTeam;

	SET_MODEL(ENT(pGrenade->pev), "models/w_hegrenade.mdl");
	pGrenade->pev->dmg = 100.0f;

	return pGrenade;
}

CGrenade *CGrenade::ShootTimed(entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time)
{
	CGrenade *pGrenade = GetClassPtr<CCSGrenade>((CGrenade *)NULL);
	pGrenade->Spawn();

	UTIL_SetOrigin(pGrenade->pev, vecStart);
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = pevOwner->angles;
	pGrenade->pev->owner = ENT(pevOwner);

	// Bounce if touched
	pGrenade->SetTouch(&CGrenade::BounceTouch);

	// Take one second off of the desired detonation time and set the think to PreDetonate. PreDetonate
	// will insert a DANGER sound into the world sound list and delay detonation for one second so that
	// the grenade explodes after the exact amount of time specified in the call to ShootTimed().

	pGrenade->pev->dmgtime = gpGlobals->time + time;
	pGrenade->SetThink(&CGrenade::TumbleThink);
	pGrenade->pev->nextthink = gpGlobals->time + 0.1f;

	if (time < 0.1f)
	{
		pGrenade->pev->nextthink = gpGlobals->time;
		pGrenade->pev->velocity = Vector(0, 0, 0);
	}

	pGrenade->pev->sequence = RANDOM_LONG(3, 6);
	pGrenade->pev->framerate = 1.0f;

	pGrenade->m_bJustBlew = true;

	pGrenade->pev->gravity = 0.5f;
	pGrenade->pev->friction = 0.8f;

	SET_MODEL(ENT(pGrenade->pev), "models/w_flashbang.mdl");
	pGrenade->pev->dmg = 35.0f;

	return pGrenade;
}

void CGrenade::__MAKE_VHOOK(Use)(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (!m_bIsC4)
		return;

	CBasePlayer *player = GetClassPtr<CCSPlayer>((CBasePlayer *)pActivator->pev);

	// For CTs to defuse the c4
	if (player->m_iTeam != CT)
	{
		return;
	}

	if (m_bStartDefuse)
	{
		m_fNextDefuse = gpGlobals->time + 0.5f;
		return;
	}

	// freeze the player in place while defusing
	SET_CLIENT_MAXSPEED(player->edict(), 1);

	if (TheBots)
	{
		TheBots->OnEvent(EVENT_BOMB_DEFUSING, pActivator);
	}

	if (CSGameRules()->IsCareer())
	{
		if (TheCareerTasks)
		{
			TheCareerTasks->HandleEvent(EVENT_BOMB_DEFUSING);
		}
	}

	if (player->m_bHasDefuser)
	{
		UTIL_LogPrintf("\"%s<%i><%s><CT>\" triggered \"Begin_Bomb_Defuse_With_Kit\"\n",
			STRING(player->pev->netname),
			GETPLAYERUSERID(player->edict()),
			GETPLAYERAUTHID(player->edict()));

		// TODO show messages on clients on event
		ClientPrint(player->pev, HUD_PRINTCENTER, "#Defusing_Bomb_With_Defuse_Kit");
		EMIT_SOUND(ENT(player->pev), CHAN_ITEM, "weapons/c4_disarm.wav", VOL_NORM, ATTN_NORM);

		player->m_bIsDefusing = true;
		m_pBombDefuser = pActivator;
		m_bStartDefuse = true;
		m_flDefuseCountDown = gpGlobals->time + 5.0f;
		m_fNextDefuse = gpGlobals->time + 0.5f;

		// start the progress bar
		player->SetProgressBarTime(5);
	}
	else
	{
		UTIL_LogPrintf("\"%s<%i><%s><CT>\" triggered \"Begin_Bomb_Defuse_Without_Kit\"\n",
			STRING(player->pev->netname),
			GETPLAYERUSERID(player->edict()),
			GETPLAYERAUTHID(player->edict()));

		// TODO: show messages on clients on event
		ClientPrint(player->pev, HUD_PRINTCENTER, "#Defusing_Bomb_Without_Defuse_Kit");
		EMIT_SOUND(ENT(player->pev), CHAN_ITEM, "weapons/c4_disarm.wav", VOL_NORM, ATTN_NORM);

		player->m_bIsDefusing = true;
		m_pBombDefuser = pActivator;
		m_bStartDefuse = true;
		m_flDefuseCountDown = gpGlobals->time + 10.0f;
		m_fNextDefuse = gpGlobals->time + 0.5f;

		// start the progress bar
		player->SetProgressBarTime(10);
	}
}

CGrenade *CGrenade::ShootSatchelCharge(entvars_t *pevOwner, Vector vecStart, Vector vecAngles)
{
	CGrenade *pGrenade = GetClassPtr<CCSGrenade>((CGrenade *)NULL);
	pGrenade->pev->movetype = MOVETYPE_TOSS;

	MAKE_STRING_CLASS("grenade", pGrenade->pev);

	pGrenade->pev->solid = SOLID_BBOX;

	// Change this to satchel charge model
	SET_MODEL(ENT(pGrenade->pev), "models/w_c4.mdl");

	UTIL_SetSize(pGrenade->pev, Vector(-3, -6, 0), Vector(3, 6, 8));

	pGrenade->pev->dmg = 100.0f;
	UTIL_SetOrigin(pGrenade->pev, vecStart);
	pGrenade->pev->velocity = g_vecZero;
	pGrenade->pev->angles = vecAngles;
	pGrenade->pev->owner = ENT(pevOwner);

	// Detonate in "time" seconds
	pGrenade->SetThink(&CGrenade::C4Think);
	pGrenade->SetTouch(&CGrenade::C4Touch);
	pGrenade->pev->spawnflags = SF_DETONATE;

	pGrenade->pev->nextthink = gpGlobals->time + 0.1f;
	pGrenade->m_flC4Blow = gpGlobals->time + CSGameRules()->m_iC4Timer;
	pGrenade->m_flNextFreqInterval = float(CSGameRules()->m_iC4Timer / 4);
	pGrenade->m_flNextFreq = gpGlobals->time;

	pGrenade->m_iCurWave = 0;
	pGrenade->m_fAttenu = 0;
	pGrenade->m_sBeepName = NULL;
	pGrenade->m_flNextBeep = gpGlobals->time + 0.5f;
	pGrenade->m_bIsC4 = true;
	pGrenade->m_fNextDefuse = 0;
	pGrenade->m_bStartDefuse = false;
	pGrenade->m_flNextBlink = gpGlobals->time + 2.0f;

	pGrenade->pev->friction = 0.9f;
	pGrenade->m_bJustBlew = false;

	CBasePlayer *pOwner = CBasePlayer::Instance(pevOwner);
	if (pOwner && pOwner->IsPlayer())
	{
		pGrenade->m_pentCurBombTarget = pOwner->m_pentCurBombTarget;
	}
	else
		pGrenade->m_pentCurBombTarget = NULL;

	return pGrenade;
}

CGrenade *CGrenade::ShootSmokeGrenade(entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time, unsigned short usEvent)
{
	CGrenade *pGrenade = GetClassPtr<CCSGrenade>((CGrenade *)NULL);
	pGrenade->Spawn();

	UTIL_SetOrigin(pGrenade->pev, vecStart);
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = pevOwner->angles;
	pGrenade->pev->owner = ENT(pevOwner);
	pGrenade->m_usEvent = usEvent;
	pGrenade->m_bLightSmoke = false;
	pGrenade->m_bDetonated = false;
	pGrenade->SetTouch(&CGrenade::BounceTouch);
	pGrenade->pev->dmgtime = gpGlobals->time + time;
	pGrenade->SetThink(&CGrenade::SG_TumbleThink);
	pGrenade->pev->nextthink = gpGlobals->time + 0.1f;

	if (time < 0.1)
	{
		pGrenade->pev->nextthink = gpGlobals->time;
		pGrenade->pev->velocity = Vector(0, 0, 0);
	}

	pGrenade->pev->sequence = RANDOM_LONG(3, 6);
	pGrenade->pev->framerate = 1.0f;
	pGrenade->m_bJustBlew = true;
	pGrenade->pev->gravity = 0.5f;
	pGrenade->pev->friction = 0.8f;
	pGrenade->m_SGSmoke = 0;

	SET_MODEL(ENT(pGrenade->pev), "models/w_smokegrenade.mdl");
	pGrenade->pev->dmg = 35.0f;

	return pGrenade;
}

void AnnounceFlashInterval(float interval, float offset)
{
	if (!g_bIsCzeroGame)
		return;

	MESSAGE_BEGIN(MSG_ALL, gmsgScenarioIcon);
		WRITE_BYTE(1);
		WRITE_STRING("bombticking");
		WRITE_BYTE(255);
		WRITE_SHORT(int(interval));	// interval
		WRITE_SHORT(int(offset));
	MESSAGE_END();
}

void CGrenade::C4Think()
{
	if (!IsInWorld())
	{
		UTIL_Remove(this);
		return;
	}

	pev->nextthink = gpGlobals->time + 0.12f;

	if (gpGlobals->time >= m_flNextFreq)
	{
		m_flNextFreq = gpGlobals->time + m_flNextFreqInterval;
		m_flNextFreqInterval *= 0.9f;

		switch (m_iCurWave)
		{
		case 0:
			m_sBeepName = "weapons/c4_beep1.wav";
			m_fAttenu = 1.5f;

			AnnounceFlashInterval(139);
			break;
		case 1:
			m_sBeepName = "weapons/c4_beep2.wav";
			m_fAttenu = 1.0f;

			AnnounceFlashInterval(69, 10);
			break;
		case 2:
			m_sBeepName = "weapons/c4_beep3.wav";
			m_fAttenu = 0.8f;

			AnnounceFlashInterval(40);
			break;
		case 3:
			m_sBeepName = "weapons/c4_beep4.wav";
			m_fAttenu = 0.5f;

			AnnounceFlashInterval(30);
			break;
		case 4:
			m_sBeepName = "weapons/c4_beep5.wav";
			m_fAttenu = 0.2f;

			AnnounceFlashInterval(20);
			break;
		}

		++m_iCurWave;
	}

	if (gpGlobals->time >= m_flNextBeep)
	{
		m_flNextBeep = gpGlobals->time + 1.4f;
		EMIT_SOUND(ENT(pev), CHAN_VOICE, m_sBeepName, VOL_NORM, m_fAttenu);

		// let the bots hear the bomb beeping
		// BOTPORT: Emit beep events at same time as client effects
		if (TheBots)
		{
			TheBots->OnEvent(EVENT_BOMB_BEEP, this);
		}
	}

	if (gpGlobals->time >= m_flNextBlink)
	{
		m_flNextBlink = gpGlobals->time + 2.0f;

		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
			WRITE_BYTE(TE_GLOWSPRITE);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z + 5.0f);
			WRITE_SHORT(g_sModelIndexC4Glow);
			WRITE_BYTE(1);
			WRITE_BYTE(3);
			WRITE_BYTE(255);
		MESSAGE_END();
	}

	// If the timer has expired ! blow this bomb up!
	if (gpGlobals->time >= m_flC4Blow)
	{
		if (TheBots)
		{
			TheBots->OnEvent(EVENT_BOMB_EXPLODED);
		}

		MESSAGE_BEGIN(MSG_ALL, gmsgScenarioIcon);
			WRITE_BYTE(0);
		MESSAGE_END();

		if (m_pentCurBombTarget)
		{
			CBaseEntity *pBombTarget = CBaseEntity::Instance(m_pentCurBombTarget);
			if (pBombTarget)
			{
				pBombTarget->Use(CBaseEntity::Instance(pev->owner), this, USE_TOGGLE, 0);
			}
		}

		CBasePlayer *pBombOwner = CBasePlayer::Instance(pev->owner);
		if (pBombOwner)
		{
			pBombOwner->pev->frags += 3.0f;
		}

		MESSAGE_BEGIN(MSG_ALL, gmsgBombPickup);
		MESSAGE_END();

		g_pGameRules->m_bBombDropped = FALSE;

		if (pev->waterlevel != 0)
			UTIL_Remove(this);
		else
			SetThink(&CGrenade::Detonate2);
	}

	// if the defusing process has started
	if (m_bStartDefuse && m_pBombDefuser)
	{
		CBasePlayer *pPlayer = (CBasePlayer *)m_pBombDefuser;

		// if the defusing process has not ended yet
		if (gpGlobals->time < m_flDefuseCountDown)
		{
			int iOnGround = ((m_pBombDefuser->pev->flags & FL_ONGROUND) == FL_ONGROUND);

			// if the bomb defuser has stopped defusing the bomb
			if (gpGlobals->time > m_fNextDefuse || !iOnGround)
			{
				if (!iOnGround)
				{
					ClientPrint(m_pBombDefuser->pev, HUD_PRINTCENTER, "#C4_Defuse_Must_Be_On_Ground");
				}

				// release the player from being frozen
				pPlayer->ResetMaxSpeed();
				pPlayer->m_bIsDefusing = false;

				// cancel the progress bar
				pPlayer->SetProgressBarTime(0);
				m_pBombDefuser = NULL;
				m_bStartDefuse = false;
				m_flDefuseCountDown = 0;

				// tell the bots someone has aborted defusing
				if (TheBots)
				{
					TheBots->OnEvent(EVENT_BOMB_DEFUSE_ABORTED);
				}
			}
		}
		// if the defuse process has ended, kill the c4
		else if (m_pBombDefuser->pev->deadflag == DEAD_NO)
		{
			Broadcast("BOMBDEF");

			if (TheBots)
			{
				TheBots->OnEvent(EVENT_BOMB_DEFUSED, (CBaseEntity *)m_pBombDefuser);
			}

			MESSAGE_BEGIN(MSG_SPEC, SVC_DIRECTOR);
				WRITE_BYTE(9);
				WRITE_BYTE(DRC_CMD_EVENT);
				WRITE_SHORT(ENTINDEX(m_pBombDefuser->edict()));
				WRITE_SHORT(0);
				WRITE_LONG(15 | DRC_FLAG_FINAL | DRC_FLAG_FACEPLAYER | DRC_FLAG_DRAMATIC);
			MESSAGE_END();

			UTIL_LogPrintf("\"%s<%i><%s><CT>\" triggered \"Defused_The_Bomb\"\n",
				STRING(m_pBombDefuser->pev->netname),
				GETPLAYERUSERID(m_pBombDefuser->edict()),
				GETPLAYERAUTHID(m_pBombDefuser->edict()));

			UTIL_EmitAmbientSound(ENT(pev), pev->origin, "weapons/c4_beep5.wav", 0, ATTN_NONE, SND_STOP, 0);
			EMIT_SOUND(ENT(m_pBombDefuser->pev), CHAN_WEAPON, "weapons/c4_disarmed.wav", VOL_NORM, ATTN_NORM);
			UTIL_Remove(this);

			m_bJustBlew = true;

			// release the player from being frozen
			pPlayer->ResetMaxSpeed();
			pPlayer->m_bIsDefusing = false;

			MESSAGE_BEGIN(MSG_ALL, gmsgScenarioIcon);
				WRITE_BYTE(0);
			MESSAGE_END();

			if (CSGameRules()->IsCareer() && !pPlayer->IsBot())
			{
				if (TheCareerTasks)
				{
					TheCareerTasks->HandleEvent(EVENT_BOMB_DEFUSED, pPlayer);
				}
			}

			CSGameRules()->m_bBombDefused = true;
			CSGameRules()->CheckWinConditions();

			// give the defuser credit for defusing the bomb
			m_pBombDefuser->pev->frags += 3.0f;

			MESSAGE_BEGIN(MSG_ALL, gmsgBombPickup);
			MESSAGE_END();

			g_pGameRules->m_bBombDropped = FALSE;
			m_pBombDefuser = NULL;
			m_bStartDefuse = false;
		}
		else
		{
			// if it gets here then the previouse defuser has taken off or been killed
			// release the player from being frozen
			pPlayer->ResetMaxSpeed();
			pPlayer->m_bIsDefusing = false;

			m_bStartDefuse = false;
			m_pBombDefuser = NULL;

			// tell the bots someone has aborted defusing
			if (TheBots)
			{
				TheBots->OnEvent(EVENT_BOMB_DEFUSE_ABORTED);
			}
		}
	}
}

void CGrenade::C4Touch(CBaseEntity *pOther)
{
	;
}

NOXREF void CGrenade::UseSatchelCharges(entvars_t *pevOwner, SATCHELCODE code)
{
	if (!pevOwner)
		return;

	edict_t *pentFind = NULL;
	CBaseEntity *pOwner = CBaseEntity::Instance(pevOwner);

	while ((pentFind = FIND_ENTITY_BY_CLASSNAME(pentFind, "grenade")))
	{
		if (FNullEnt(pentFind))
			break;

		CBaseEntity *pEnt = Instance(pentFind);
		if (pEnt)
		{
			if ((pEnt->pev->spawnflags & SF_DETONATE) && pEnt->pev->owner == pOwner->edict())
			{
				if (code == SATCHEL_DETONATE)
					pEnt->Use(pOwner, pOwner, USE_ON, 0);
				else
				{
					// SATCHEL_RELEASE
					pEnt->pev->owner = NULL;
				}
			}
		}
	}
}

IMPLEMENT_SAVERESTORE(CGrenade, CBaseMonster)
