// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

CMapInfo *g_pMapInfo = nullptr;

CMapInfo::CMapInfo()
{
	m_flBombRadius = 500.0f;
	m_iBuyingStatus = BUYING_EVERYONE;

#ifdef REGAMEDLL_ADD
	if (g_pMapInfo)
	{
		// Should only be one of these.
		ALERT(at_warning, "Warning: Multiple info_map_parameters entities in map!\n");
	}
	else
#endif
	{
		g_pMapInfo = this;
	}
}

void CMapInfo::CheckMapInfo()
{
	bool bCTCantBuy, bTCantBuy;
	switch (m_iBuyingStatus)
	{
	case BUYING_EVERYONE:
		bCTCantBuy = false;
		bTCantBuy = false;

		ALERT(at_console, "EVERYONE CAN BUY!\n");
		break;
	case BUYING_ONLY_CTS:
		bCTCantBuy = false;
		bTCantBuy = true;

		ALERT(at_console, "Only CT's can buy!!\n");
		break;
	case BUYING_ONLY_TERRORISTS:
		bCTCantBuy = true;
		bTCantBuy = false;

		ALERT(at_console, "Only T's can buy!!\n");
		break;
	case BUYING_NO_ONE:
		bCTCantBuy = true;
		bTCantBuy = true;

		ALERT(at_console, "No one can buy!!\n");
		break;
	default:
		bCTCantBuy = false;
		bTCantBuy = false;
		break;
	}

	CSGameRules()->m_flBombRadius = m_flBombRadius;
	CSGameRules()->m_bCTCantBuy = bCTCantBuy;
	CSGameRules()->m_bTCantBuy = bTCantBuy;
}

void CMapInfo::__MAKE_VHOOK(KeyValue)(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "buying"))
	{
		m_iBuyingStatus = (InfoMapBuyParam)Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "bombradius"))
	{
		m_flBombRadius = (float)Q_atoi(pkvd->szValue);

		if (m_flBombRadius > MAX_BOMB_RADIUS)
			m_flBombRadius = MAX_BOMB_RADIUS;

		pkvd->fHandled = TRUE;
	}
}

void CMapInfo::__MAKE_VHOOK(Spawn)()
{
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->effects |= EF_NODRAW;
}

void CMapInfo::UpdateOnRemove()
{
	g_pMapInfo = nullptr;
}

LINK_ENTITY_TO_CLASS(info_map_parameters, CMapInfo, CCSMapInfo)
