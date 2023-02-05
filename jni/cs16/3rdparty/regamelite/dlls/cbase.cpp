// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

/*
* Globals initialization
*/
#ifndef HOOK_GAMEDLL

DLL_FUNCTIONS gFunctionTable =
{
	&GameDLLInit,
	&DispatchSpawn,
	&DispatchThink,
	&DispatchUse,
	&DispatchTouch,
	&DispatchBlocked,
	&DispatchKeyValue,
	&DispatchSave,
	&DispatchRestore,
	&DispatchObjectCollsionBox,
	&SaveWriteFields,
	&SaveReadFields,
	&SaveGlobalState,
	&RestoreGlobalState,
	&ResetGlobalState,
	&ClientConnect,
	&ClientDisconnect,
	&ClientKill,
	&ClientPutInServer,
	&ClientCommand_,
	&ClientUserInfoChanged,
	&ServerActivate,
	&ServerDeactivate,
	&PlayerPreThink,
	&PlayerPostThink,
	&StartFrame,
	&ParmsNewLevel,
	&ParmsChangeLevel,
	&GetGameDescription,
	&PlayerCustomization,
	&SpectatorConnect,
	&SpectatorDisconnect,
	&SpectatorThink,
	&SysEngine_Error,
	&PM_Move,
	&PM_Init,
	&PM_FindTextureType,
	&SetupVisibility,
	&UpdateClientData,
	&AddToFullPack,
	&CreateBaseline,
	&RegisterEncoders,
	&GetWeaponData,
	&CmdStart,
	&CmdEnd,
	&ConnectionlessPacket,
	&GetHullBounds,
	&CreateInstancedBaselines,
	&InconsistentFile,
	&AllowLagCompensation
};

NEW_DLL_FUNCTIONS gNewDLLFunctions =
{
	&OnFreeEntPrivateData,
	NULL,
	NULL,
	NULL,
	NULL
};

// Global Savedata for Delay
TYPEDESCRIPTION	CBaseEntity::m_SaveData[] =
{
	DEFINE_FIELD(CBaseEntity, m_pGoalEnt, FIELD_CLASSPTR),
	DEFINE_FIELD(CBaseEntity, m_pfnThink, FIELD_FUNCTION),	// UNDONE: Build table of these!
	DEFINE_FIELD(CBaseEntity, m_pfnTouch, FIELD_FUNCTION),
	DEFINE_FIELD(CBaseEntity, m_pfnUse, FIELD_FUNCTION),
	DEFINE_FIELD(CBaseEntity, m_pfnBlocked, FIELD_FUNCTION),
};

CMemoryPool hashItemMemPool(sizeof(hash_item_t), 64);

#endif // HOOK_GAMEDLL

int CaseInsensitiveHash(const char *string, int iBounds)
{
	unsigned int hash = 0;

	if (!*string)
		return 0;

	while (*string)
	{
		if (*string < 'A' || *string > 'Z')
			hash = *string + 2 * hash;
		else
			hash = *string + 2 * hash + ' ';

		string++;
	}

	return (hash % iBounds);
}

void EmptyEntityHashTable()
{
	int i;
	hash_item_t *item, *temp, *free;

	for (i = 0; i < stringsHashTable.Count(); ++i)
	{
		item = &stringsHashTable[i];
		temp = item->next;

		item->pev = NULL;
		item->pevIndex = 0;
		item->lastHash = 0;
		item->next = NULL;

		while (temp)
		{
			free = temp;
			temp = temp->next;
			hashItemMemPool.Free(free);
		}
	}
}

void EXT_FUNC AddEntityHashValue(entvars_t *pev, const char *value, hash_types_e fieldType)
{
	int count;
	hash_item_t *item, *next, *temp, *newp;
	int hash, pevIndex;
	entvars_t *pevtemp;

	if (fieldType != CLASSNAME)
		return;

	if (FStringNull(pev->classname))
		return;

	count = stringsHashTable.Count();
	hash = CaseInsensitiveHash(value, count);
	pevIndex = ENTINDEX(ENT(pev));
	item = &stringsHashTable[hash];

	while (item->pev)
	{
		if (!Q_strcmp(STRING(item->pev->classname), STRING(pev->classname)))
			break;

		hash = (hash + 1) % count;
		item = &stringsHashTable[hash];
	}
	if (item->pev)
	{
		next = item->next;
		while (next)
		{
			if (item->pev == pev || item->pevIndex >= pevIndex)
				break;

			item = next;
			next = next->next;
		}
		if (pevIndex < item->pevIndex)
		{
			pevtemp = item->pev;
			item->pev = pev;
			item->lastHash = NULL;
			item->pevIndex = pevIndex;

			pevIndex = ENTINDEX(ENT(pevtemp));
		}
		else
			pevtemp = pev;

		if (item->pev != pevtemp)
		{
			temp = item->next;
			newp = (hash_item_t *)hashItemMemPool.Alloc(sizeof(hash_item_t));

			item->next = newp;
			newp->pev = pevtemp;
			newp->lastHash = NULL;
			newp->pevIndex = pevIndex;

			if (next)
				newp->next = temp;
			else
				newp->next = NULL;
		}
	}
	else
	{
		item->pev = pev;
		item->lastHash = NULL;
		item->pevIndex = ENTINDEX(ENT(pev));
	}
}

void EXT_FUNC RemoveEntityHashValue(entvars_t *pev, const char *value, hash_types_e fieldType)
{
	int hash;
	hash_item_t *item;
	hash_item_t *last;
	int pevIndex;
	int count;

	count = stringsHashTable.Count();
	hash = CaseInsensitiveHash(value, count);
	pevIndex = ENTINDEX(ENT(pev));

	if (fieldType != CLASSNAME)
		return;

	hash = hash % count;
	item = &stringsHashTable[hash];

	while (item->pev)
	{
		if (!Q_strcmp(STRING(item->pev->classname), STRING(pev->classname)))
			break;

		hash = (hash + 1) % count;
		item = &stringsHashTable[hash];
	}
	if (item->pev)
	{
		last = item;
		while (item->next)
		{
			if (item->pev == pev)
				break;

			last = item;
			item = item->next;
		}
		if (item->pev == pev)
		{
			if (last == item)
			{
				if (item->next)
				{
					item->pev = item->next->pev;
					item->pevIndex = item->next->pevIndex;
					item->lastHash = NULL;
					item->next = item->next->next;
				}
				else
				{
					item->pev = NULL;
					item->lastHash = NULL;
					item->pevIndex = 0;
				}
			}
			else
			{
				if (stringsHashTable[hash].lastHash == item)
					stringsHashTable[hash].lastHash = NULL;

				last->next = item->next;
				hashItemMemPool.Free(item);
			}
		}
	}
}

void printEntities()
{
	for (int i = 0; i < stringsHashTable.Count(); ++i)
	{
		hash_item_t *item = &stringsHashTable[i];

		if (item->pev)
		{
			UTIL_LogPrintf("Print: %s %i %p\n", STRING(stringsHashTable[i].pev->classname), ENTINDEX(ENT(item->pev)), item->pev);
		}

		for (item = stringsHashTable[i].next; item; item = item->next)
		{
			UTIL_LogPrintf("Print: %s %i %p\n", STRING(item->pev->classname), ENTINDEX(ENT(item->pev)), item->pev);
		}
	}
}

NOINLINE edict_t *EXT_FUNC CREATE_NAMED_ENTITY(string_t iClass)
{
	edict_t *named = g_engfuncs.pfnCreateNamedEntity(iClass);
	if (named)
	{
		AddEntityHashValue(&named->v, STRING(iClass), CLASSNAME);
	}

	return named;
}

void REMOVE_ENTITY(edict_t *pEntity)
{
	if (pEntity)
	{
		g_engfuncs.pfnRemoveEntity(pEntity);
	}
}

void CONSOLE_ECHO_(char *pszMsg, ...)
{
	va_list argptr;
	static char szStr[1024];

	va_start(argptr, pszMsg);
	vsprintf(szStr, pszMsg, argptr);
	va_end(argptr);

	SERVER_PRINT(szStr);
}

void loopPerformance()
{
	CPerformanceCounter loopCounter;
	loopCounter.InitializePerformanceCounter();

	double start, end;
	int i;

	start = loopCounter.GetCurTime();

	for (i = 0; i < 100; ++i)
	{
		CBaseEntity *pSpot;
		for (pSpot = UTIL_FindEntityByString_Old(NULL, "classname", "info_player_start"); pSpot; pSpot = UTIL_FindEntityByString_Old(pSpot, "classname", "info_player_start"))
			;

		for (pSpot = UTIL_FindEntityByString_Old(NULL, "classname", "info_player_deathmatch"); pSpot; pSpot = UTIL_FindEntityByString_Old(pSpot, "classname", "info_player_deathmatch"))
			;

		for (pSpot = UTIL_FindEntityByString_Old(NULL, "classname", "player"); pSpot; pSpot = UTIL_FindEntityByString_Old(pSpot, "classname", "player"))
			;

		for (pSpot = UTIL_FindEntityByString_Old(NULL, "classname", "bodyque"); pSpot; pSpot = UTIL_FindEntityByString_Old(pSpot, "classname", "bodyque"))
			;
	}

	end = loopCounter.GetCurTime();
	CONSOLE_ECHO(" Time in old search loop %.4f\n", (end - start) * 1000.0);

	// check time new search loop
	start = loopCounter.GetCurTime();

	for (i = 0; i < 100; ++i)
	{
		CBaseEntity *pSpot;
		for (pSpot = UTIL_FindEntityByString(NULL, "classname", "info_player_start"); pSpot; pSpot = UTIL_FindEntityByString(pSpot, "classname", "info_player_start"))
			;

		for (pSpot = UTIL_FindEntityByString(NULL, "classname", "info_player_deathmatch"); pSpot; pSpot = UTIL_FindEntityByString(pSpot, "classname", "info_player_deathmatch"))
			;

		for (pSpot = UTIL_FindEntityByString(NULL, "classname", "player"); pSpot; pSpot = UTIL_FindEntityByString(pSpot, "classname", "player"))
			;

		for (pSpot = UTIL_FindEntityByString(NULL, "classname", "bodyque"); pSpot; pSpot = UTIL_FindEntityByString(pSpot, "classname", "bodyque"))
			;
	}

	end = loopCounter.GetCurTime();
	CONSOLE_ECHO(" Time in new search loop %.4f\n", (end - start) * 1000.0);
}

C_DLLEXPORT int GetEntityAPI(DLL_FUNCTIONS *pFunctionTable, int interfaceVersion)
{
	if (!pFunctionTable || interfaceVersion != INTERFACE_VERSION)
		return 0;

	Q_memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	stringsHashTable.AddMultipleToTail(2048);
	for (int i = 0; i < stringsHashTable.Count(); ++i)
	{
		stringsHashTable[i].next = NULL;
	}

	EmptyEntityHashTable();
	return 1;
}

NOXREF int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	if (!pFunctionTable || *interfaceVersion != INTERFACE_VERSION)
	{
		*interfaceVersion = INTERFACE_VERSION;
		return 0;
	}

	Q_memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));
	return 1;
}

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
	if (!pFunctionTable || *interfaceVersion != NEW_DLL_FUNCTIONS_VERSION)
	{
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return 0;
	}

	Q_memcpy(pFunctionTable, &gNewDLLFunctions, sizeof(NEW_DLL_FUNCTIONS));
	return 1;
}

int EXT_FUNC DispatchSpawn(edict_t *pent)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);

	if (pEntity)
	{
		// Initialize these or entities who don't link to the world won't have anything in here
		pEntity->pev->absmin = pEntity->pev->origin - Vector(1, 1, 1);
		pEntity->pev->absmax = pEntity->pev->origin + Vector(1, 1, 1);
		pEntity->Spawn();

		// Try to get the pointer again, in case the spawn function deleted the entity.
		// UNDONE: Spawn() should really return a code to ask that the entity be deleted, but
		// that would touch too much code for me to do that right now.
		pEntity = (CBaseEntity *)GET_PRIVATE(pent);

		if (pEntity)
		{
			if (g_pGameRules && !g_pGameRules->IsAllowedToSpawn(pEntity))
			{
				// return that this entity should be deleted
				return -1;
			}

			if (pEntity->pev->flags & FL_KILLME)
			{
				return -1;
			}
		}

		// Handle global stuff here
		if (pEntity && pEntity->pev->globalname)
		{
			const globalentity_t *pGlobal = gGlobalState.EntityFromTable(pEntity->pev->globalname);

			if (pGlobal)
			{
				// Already dead? delete
				if (pGlobal->state == GLOBAL_DEAD)
					return -1;

				else if (!FStrEq(STRING(gpGlobals->mapname), pGlobal->levelName))
				{
					// Hasn't been moved to this level yet, wait but stay alive
					// In this level & not dead, continue on as normal
					pEntity->MakeDormant();
				}
			}
			else
			{
				// Spawned entities default to 'On'
				gGlobalState.EntityAdd(pEntity->pev->globalname, gpGlobals->mapname, GLOBAL_ON);
			}
		}

	}

	return 0;
}

void EXT_FUNC DispatchKeyValue(edict_t *pentKeyvalue, KeyValueData *pkvd)
{
	if (!pkvd || !pentKeyvalue)
		return;

	EntvarsKeyvalue(VARS(pentKeyvalue), pkvd);

	// If the key was an entity variable, or there's no class set yet, don't look for the object, it may
	// not exist yet.
	if (pkvd->fHandled || !pkvd->szClassName)
		return;

	// Get the actualy entity object
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentKeyvalue);
	if (!pEntity)
		return;

	pEntity->KeyValue(pkvd);
}

// HACKHACK -- this is a hack to keep the node graph entity from "touching" things (like triggers)
// while it builds the graph
void EXT_FUNC DispatchTouch(edict_t *pentTouched, edict_t *pentOther)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentTouched);
	CBaseEntity *pOther = (CBaseEntity *)GET_PRIVATE(pentOther);

	if (pEntity && pOther && !((pEntity->pev->flags | pOther->pev->flags) & FL_KILLME))
		pEntity->Touch(pOther);
}

void EXT_FUNC DispatchUse(edict_t *pentUsed, edict_t *pentOther)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentUsed);
	CBaseEntity *pOther = (CBaseEntity *)GET_PRIVATE(pentOther);

	if (pEntity && !(pEntity->pev->flags & FL_KILLME))
		pEntity->Use(pOther, pOther, USE_TOGGLE, 0);
}

void EXT_FUNC DispatchThink(edict_t *pent)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);

	if (pEntity)
	{
		if (pEntity->pev->flags & FL_DORMANT)
		{
			ALERT(at_error, "Dormant entity %s is thinking!!\n", STRING(pEntity->pev->classname));
		}

		pEntity->Think();
	}
}

void EXT_FUNC DispatchBlocked(edict_t *pentBlocked, edict_t *pentOther)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentBlocked);
	CBaseEntity *pOther = (CBaseEntity *)GET_PRIVATE(pentOther);

	if (pEntity)
	{
		pEntity->Blocked(pOther);
	}
}

void EXT_FUNC DispatchSave(edict_t *pent, SAVERESTOREDATA *pSaveData)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);

	if (pEntity && pSaveData)
	{
		ENTITYTABLE *pTable = &pSaveData->pTable[ pSaveData->currentIndex ];

		if (pTable->pent != pent)
		{
			ALERT(at_error, "ENTITY TABLE OR INDEX IS WRONG!!!!\n");
		}

		if (pEntity->ObjectCaps() & FCAP_DONT_SAVE)
			return;

		// These don't use ltime & nextthink as times really, but we'll fudge around it.
		if (pEntity->pev->movetype == MOVETYPE_PUSH)
		{
			float_precision delta = pEntity->pev->nextthink - pEntity->pev->ltime;
			pEntity->pev->ltime = gpGlobals->time;
			pEntity->pev->nextthink = pEntity->pev->ltime + delta;
		}

		// Remember entity position for file I/O
		pTable->location = pSaveData->size;

		// Remember entity class for respawn
		pTable->classname = pEntity->pev->classname;

		CSave saveHelper(pSaveData);
		pEntity->Save(saveHelper);

		// Size of entity block is data size written to block
		pTable->size = pSaveData->size - pTable->location;
	}
}

// Find the matching global entity.  Spit out an error if the designer made entities of
// different classes with the same global name
CBaseEntity *FindGlobalEntity(string_t classname, string_t globalname)
{
	edict_t *pent = FIND_ENTITY_BY_STRING(NULL, "globalname", STRING(globalname));
	CBaseEntity *pReturn = CBaseEntity::Instance(pent);

	if (pReturn)
	{
		if (!FClassnameIs(pReturn->pev, STRING(classname)))
		{
			ALERT(at_console, "Global entity found %s, wrong class %s\n", STRING(globalname), STRING(pReturn->pev->classname));
			pReturn = NULL;
		}
	}

	return pReturn;
}

int EXT_FUNC DispatchRestore(edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);

	if (pEntity && pSaveData)
	{
		entvars_t tmpVars;
		Vector oldOffset;
		CRestore restoreHelper(pSaveData);

		if (globalEntity)
		{
			CRestore tmpRestore(pSaveData);
			tmpRestore.PrecacheMode(0);
			tmpRestore.ReadEntVars("ENTVARS", &tmpVars);

			// HACKHACK - reset the save pointers, we're going to restore for real this time
			pSaveData->size = pSaveData->pTable[pSaveData->currentIndex].location;
			pSaveData->pCurrentData = pSaveData->pBaseData + pSaveData->size;

			const globalentity_t *pGlobal = gGlobalState.EntityFromTable(tmpVars.globalname);

			// Don't overlay any instance of the global that isn't the latest
			// pSaveData->szCurrentMapName is the level this entity is coming from
			// pGlobla->levelName is the last level the global entity was active in.
			// If they aren't the same, then this global update is out of date.
			if (!FStrEq(pSaveData->szCurrentMapName, pGlobal->levelName))
			{
				return 0;
			}

			// Compute the new global offset
			oldOffset = pSaveData->vecLandmarkOffset;
			CBaseEntity *pNewEntity = FindGlobalEntity(tmpVars.classname, tmpVars.globalname);

			if (pNewEntity)
			{
				// Tell the restore code we're overlaying a global entity from another level
				// Don't overwrite global fields
				restoreHelper.SetGlobalMode(1);

				pSaveData->vecLandmarkOffset = (pSaveData->vecLandmarkOffset - pNewEntity->pev->mins) + tmpVars.mins;

				// we're going to restore this data OVER the old entity
				pEntity = pNewEntity;
				pent = ENT(pEntity->pev);

				// Update the global table to say that the global definition of this entity should come from this level
				gGlobalState.EntityUpdate(pEntity->pev->globalname, gpGlobals->mapname);
			}
			else
			{
				// This entity will be freed automatically by the engine.  If we don't do a restore on a matching entity (below)
				// or call EntityUpdate() to move it to this level, we haven't changed global state at all.

				return 0;
			}
		}

		if (pEntity->ObjectCaps() & FCAP_MUST_SPAWN)
		{
			pEntity->Restore(restoreHelper);
			pEntity->Spawn();
		}
		else
		{
			pEntity->Restore(restoreHelper);
			pEntity->Precache();
		}

		// Again, could be deleted, get the pointer again.
		pEntity = (CBaseEntity *)GET_PRIVATE(pent);

		// Is this an overriding global entity (coming over the transition), or one restoring in a level
		if (globalEntity)
		{
			pSaveData->vecLandmarkOffset = oldOffset;

			if (pEntity)
			{
				UTIL_SetOrigin(pEntity->pev, pEntity->pev->origin);
				pEntity->OverrideReset();
			}
		}
		else if (pEntity && pEntity->pev->globalname)
		{
			const globalentity_t *pGlobal = gGlobalState.EntityFromTable(pEntity->pev->globalname);

			if (pGlobal)
			{
				// Already dead? delete
				if (pGlobal->state == GLOBAL_DEAD)
					return -1;

				else if (!FStrEq(STRING(gpGlobals->mapname), pGlobal->levelName))
				{
					// Hasn't been moved to this level yet, wait but stay alive
					pEntity->MakeDormant();
				}
				// In this level & not dead, continue on as normal
			}
			else
			{
				ALERT(at_error, "Global Entity %s (%s) not in table!!!\n", STRING(pEntity->pev->globalname), STRING(pEntity->pev->classname));

				// Spawned entities default to 'On'
				gGlobalState.EntityAdd(pEntity->pev->globalname, gpGlobals->mapname, GLOBAL_ON);
			}
		}
	}

	return 0;
}

void EXT_FUNC DispatchObjectCollsionBox(edict_t *pent)
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);
	if (pEntity)
	{
		pEntity->SetObjectCollisionBox();
	}
	else
		SetObjectCollisionBox(&pent->v);

}

void EXT_FUNC SaveWriteFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount)
{
	CSave saveHelper(pSaveData);
	saveHelper.WriteFields(pname, pBaseData, pFields, fieldCount);
}

void EXT_FUNC SaveReadFields(SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount)
{
	CRestore restoreHelper(pSaveData);
	restoreHelper.ReadFields(pname, pBaseData, pFields, fieldCount);
}

edict_t *EHANDLE::Get()
{
	if (!m_pent || m_pent->serialnumber != m_serialnumber)
		return NULL;

	return m_pent;
}

edict_t *EHANDLE::Set(edict_t *pent)
{
	m_pent = pent;
	if (pent)
		m_serialnumber = pent->serialnumber;

	return pent;
}

EHANDLE::operator CBaseEntity *()
{
	return (CBaseEntity *)GET_PRIVATE(Get());
}

CBaseEntity *EHANDLE::operator=(CBaseEntity *pEntity)
{
	if (pEntity)
	{
		m_pent = ENT(pEntity->pev);
		if (m_pent)
			m_serialnumber = m_pent->serialnumber;
	}
	else
	{
		m_pent = NULL;
		m_serialnumber = 0;
	}

	return pEntity;
}

EHANDLE::operator int()
{
	return Get() != NULL;
}

CBaseEntity *EHANDLE::operator->()
{
	return (CBaseEntity *)GET_PRIVATE(Get());
}

BOOL CBaseEntity::__MAKE_VHOOK(TakeHealth)(float flHealth, int bitsDamageType)
{
	if (pev->takedamage == DAMAGE_NO)
		return FALSE;

	if (pev->health >= pev->max_health)
		return FALSE;

	pev->health += flHealth;

	if (pev->health > pev->max_health)
	{
		pev->health = pev->max_health;
	}

	return TRUE;
}

BOOL CBaseEntity::__MAKE_VHOOK(TakeDamage)(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	Vector vecTemp;

	if (pev->takedamage == DAMAGE_NO)
		return FALSE;

	// UNDONE: some entity types may be immune or resistant to some bitsDamageType
	// if Attacker == Inflictor, the attack was a melee or other instant-hit attack.
	// (that is, no actual entity projectile was involved in the attack so use the shooter's origin).
#ifndef REGAMEDLL_FIXES
	if (pevAttacker == pevInflictor)
	{
		vecTemp = pevInflictor->origin - (VecBModelOrigin(pev));
	}
	else
#endif
	{
		// an actual missile was involved.
		vecTemp = pevInflictor->origin - (VecBModelOrigin(pev));
	}

	// this global is still used for glass and other non-monster killables, along with decals.
	g_vecAttackDir = vecTemp.Normalize();

	// save damage based on the target's armor level
	// figure momentum add (don't let hurt brushes or other triggers move player)
	if ((!FNullEnt(pevInflictor)) && (pev->movetype == MOVETYPE_WALK || pev->movetype == MOVETYPE_STEP) && (pevAttacker->solid != SOLID_TRIGGER))
	{
		Vector vecDir = pev->origin - (pevInflictor->absmin + pevInflictor->absmax) * 0.5;
		vecDir = vecDir.Normalize();

		float_precision flForce = flDamage * ((32 * 32 * 72.0) / (pev->size.x * pev->size.y * pev->size.z)) * 5;

		if (flForce > 1000.0)
			flForce = 1000.0;

		pev->velocity = pev->velocity + vecDir * flForce;
	}

	// do the damage
	pev->health -= flDamage;
	if (pev->health <= 0)
	{
		Killed(pevAttacker, GIB_NORMAL);
		return FALSE;
	}

	return TRUE;
}

void CBaseEntity::__MAKE_VHOOK(Killed)(entvars_t *pevAttacker, int iGib)
{
	pev->takedamage = DAMAGE_NO;
	pev->deadflag = DEAD_DEAD;
	UTIL_Remove(this);
}

CBaseEntity *CBaseEntity::__MAKE_VHOOK(GetNextTarget)()
{
	if (FStringNull(pev->target))
		return NULL;

	edict_t *pTarget = FIND_ENTITY_BY_TARGETNAME(NULL, STRING(pev->target));
	if (FNullEnt(pTarget))
	{
		return NULL;
	}

	return Instance(pTarget);
}

int CBaseEntity::__MAKE_VHOOK(Save)(CSave &save)
{
	if (save.WriteEntVars("ENTVARS", pev))
	{
		return save.WriteFields("BASE", this, IMPL(m_SaveData), ARRAYSIZE(IMPL(m_SaveData)));
	}

	return 0;
}

int CBaseEntity::__MAKE_VHOOK(Restore)(CRestore &restore)
{
	int status = restore.ReadEntVars("ENTVARS", pev);
	if (status)
	{
		status = restore.ReadFields("BASE", this, IMPL(m_SaveData), ARRAYSIZE(IMPL(m_SaveData)));
	}

	if (pev->modelindex != 0 && !FStringNull(pev->model))
	{
		Vector mins, maxs;

		// Set model is about to destroy these
		mins = pev->mins;
		maxs = pev->maxs;

		PRECACHE_MODEL((char *)STRING(pev->model));
		SET_MODEL(ENT(pev), STRING(pev->model));

		// Reset them
		UTIL_SetSize(pev, mins, maxs);
	}

	return status;
}

// Initialize absmin & absmax to the appropriate box
void SetObjectCollisionBox(entvars_t *pev)
{
	if (pev->solid == SOLID_BSP && (pev->angles.x || pev->angles.y || pev->angles.z))
	{
		// expand for rotation
		float_precision max, v;
		int i;

		max = 0;
		for (i = 0; i < 3; ++i)
		{
			v = Q_fabs(float_precision(((float *)pev->mins)[i]));
			if (v > max)
			{
				max = v;
			}

			v = Q_fabs(float_precision(((float *)pev->maxs)[i]));
			if (v > max)
			{
				max = v;
			}
		}
		for (i = 0; i < 3; ++i)
		{
			((float *)pev->absmin)[i] = ((float *)pev->origin)[i] - max;
			((float *)pev->absmax)[i] = ((float *)pev->origin)[i] + max;
		}
	}
	else
	{
		pev->absmin = pev->origin + pev->mins;
		pev->absmax = pev->origin + pev->maxs;
	}

	pev->absmin.x -= 1;
	pev->absmin.y -= 1;
	pev->absmin.z -= 1;

	pev->absmax.x += 1;
	pev->absmax.y += 1;
	pev->absmax.z += 1;
}

void CBaseEntity::__MAKE_VHOOK(SetObjectCollisionBox)()
{
	::SetObjectCollisionBox(pev);
}

int CBaseEntity::Intersects(CBaseEntity *pOther)
{
	if (pOther->pev->absmin.x > pev->absmax.x
		|| pOther->pev->absmin.y > pev->absmax.y
		|| pOther->pev->absmin.z > pev->absmax.z
		|| pOther->pev->absmax.x < pev->absmin.x
		|| pOther->pev->absmax.y < pev->absmin.y
		|| pOther->pev->absmax.z < pev->absmin.z)
		 return 0;
	return 1;
}

void CBaseEntity::MakeDormant()
{
	pev->flags |= FL_DORMANT;
	pev->solid = SOLID_NOT;		// Don't touch
	pev->movetype = MOVETYPE_NONE;	// Don't move
	pev->effects |= EF_NODRAW;	// Don't draw
	pev->nextthink = 0;		// Don't think

	// Relink
	UTIL_SetOrigin(pev, pev->origin);
}

int CBaseEntity::IsDormant()
{
	return (pev->flags & FL_DORMANT) == FL_DORMANT;
}

BOOL CBaseEntity::__MAKE_VHOOK(IsInWorld)()
{
	// position
	if (pev->origin.x >= 4096.0 || pev->origin.y >= 4096.0 || pev->origin.z >= 4096.0)
	{
		return FALSE;
	}
	if (pev->origin.x <= -4096.0 || pev->origin.y <= -4096.0 || pev->origin.z <= -4096.0)
	{
		return FALSE;
	}

	// speed
	if (pev->velocity.x >= 2000.0 || pev->velocity.y >= 2000.0 || pev->velocity.z >= 2000.0)
	{
		return FALSE;
	}
	if (pev->velocity.x <= -2000.0 || pev->velocity.y <= -2000.0 || pev->velocity.z <= -2000.0)
	{
		return FALSE;
	}

	return TRUE;
}

int CBaseEntity::ShouldToggle(USE_TYPE useType, BOOL currentState)
{
	if (useType != USE_TOGGLE && useType != USE_SET)
	{
		if ((currentState && useType == USE_ON) || (!currentState && useType == USE_OFF))
			return 0;
	}

	return 1;
}

int CBaseEntity::__MAKE_VHOOK(DamageDecal)(int bitsDamageType)
{
	if (pev->rendermode == kRenderTransAlpha)
		return -1;

	if (pev->rendermode != kRenderNormal)
		return DECAL_BPROOF1;

	return DECAL_GUNSHOT1 + RANDOM_LONG(0, 4);
}

// NOTE: szName must be a pointer to constant memory, e.g. "monster_class" because the entity
// will keep a pointer to it after this call.
CBaseEntity *CBaseEntity::Create(char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner)
{
	edict_t	*pent = CREATE_NAMED_ENTITY(MAKE_STRING(szName));
	if (FNullEnt(pent))
	{
		ALERT(at_console, "NULL Ent in Create!\n");
		return NULL;
	}

	CBaseEntity *pEntity = Instance(pent);

	pEntity->pev->owner = pentOwner;
	pEntity->pev->origin = vecOrigin;
	pEntity->pev->angles = vecAngles;

	DispatchSpawn(pEntity->edict());

	return pEntity;
}

void EXT_FUNC OnFreeEntPrivateData(edict_t *pEnt)
{
	CBaseEntity *pEntity = CBaseEntity::Instance(pEnt);
	if (!pEntity)
		return;

	pEntity->UpdateOnRemove();
	RemoveEntityHashValue(pEntity->pev, STRING(pEntity->pev->classname), CLASSNAME);

#ifdef REGAMEDLL_API
	if (pEntity->m_pEntity) {
		delete pEntity->m_pEntity;
		pEntity->m_pEntity = nullptr;
	}
#endif

#ifdef REGAMEDLL_FIXES
	if (TheCSBots()) {
		TheCSBots()->OnFreeEntPrivateData(pEntity);
	}
#endif
}

void StudioDrawHulls( void *pmodel, void *model, edict_t *e );
void DrawDebugTriangles( void )
{
#ifdef _DEBUG
	edict_t *pEdict = INDEXENT( 2 ); // skip local
	for ( int i = 1; i < gpGlobals->maxClients; i++, pEdict++ )
	{
		if ( pEdict->free )	// Not in use
			continue;

		void *studiohdr = GET_MODEL_PTR( pEdict );

		void *model = g_physfuncs.pfnGetModel( pEdict->v.modelindex );

		if( !model || !studiohdr ) continue;

		StudioDrawHulls( studiohdr, model, pEdict );
	}
#endif
}

server_physics_api_t g_physfuncs = {};
physics_interface_t gPhysicsInterface =
{
	SV_PHYSICS_INTERFACE_VERSION,
	NULL, // SV_CreateEntity
	NULL, // SV_PhysicsEntity
	NULL, // SV_LoadEntities
	NULL, // SV_UpdatePlayerBaseVelocity
	NULL, // SV_AllowSaveGame
	NULL, // SV_TriggerTouch
	NULL, // SV_CheckFeatures
	DrawDebugTriangles, //DrawDebugTriangles,
	NULL, // DrawNormalTriangles
	NULL, // DrawOrthoTriangles
	NULL, // ClipMoveToEntity
	NULL, // ClipPMoveToEntity
	NULL, // SV_EndFrame
	NULL, // pfnCreateEntitiesInTransitionList
	NULL, // pfnCreateEntitiesInRestoreList
	NULL, // pfnAllocString
	NULL, // pfnMakeString
	NULL, // pfnGetString
	NULL  // pfnRestoreDecal
};

extern "C" EXPORT int Server_GetPhysicsInterface( int iVersion, server_physics_api_t *pfuncsFromEngine, physics_interface_t *pFunctionTable )
{
	if ( !pFunctionTable || !pfuncsFromEngine || iVersion != SV_PHYSICS_INTERFACE_VERSION )
	{
		return FALSE;
	}

	size_t iExportSize = sizeof( server_physics_api_t );
	size_t iImportSize = sizeof( physics_interface_t );

	// NOTE: the very old versions NOT have information about current build in any case
	if( g_iXashEngineBuildNumber <= 1910 )
	{
		ALERT( at_console, "old version of Xash3D was detected. Engine features was disabled.\n" );

		// interface sizes for build 1905 and older
		iExportSize = 28;
		iImportSize = 24;
	}

	// copy new physics interface
	memcpy( &g_physfuncs, pfuncsFromEngine, iExportSize );

	// fill engine callbacks
	memcpy( pFunctionTable, &gPhysicsInterface, iImportSize );

	g_fPhysicInitialized = TRUE;

	return TRUE;
}
