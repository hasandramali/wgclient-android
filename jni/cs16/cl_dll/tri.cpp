//========= Copyright ? 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any
#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "rain.h"
#include "particlemgr.h"

extern float g_fFogColor[3];
extern float g_fStartDist;
extern float g_fEndDist;
extern int g_iWaterLevel;

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles( void )
{
	gHUD.m_Spectator.DrawOverview();
}

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void DLLEXPORT HUD_DrawTransparentTriangles( void )
{
	ProcessFXObjects();
	ProcessRain();
	DrawRain();
	DrawFXObjects();
}

void BlackFog ( void )
{
	//Not in water and we want fog.
	static float fColorBlack[3] = {0,0,0};
	bool bFog = g_iWaterLevel < 2 && g_fStartDist > 0 && g_fEndDist > 0;
	if (bFog)
		gEngfuncs.pTriAPI->Fog ( fColorBlack, g_fStartDist, g_fEndDist, bFog );
	else
		gEngfuncs.pTriAPI->Fog ( g_fFogColor, g_fStartDist, g_fEndDist, bFog );
}

void RenderFog ( void )
{
	//Not in water and we want fog.
	bool bFog = g_iWaterLevel < 2 && g_fStartDist > 0 && g_fEndDist > 0;
	if (bFog)
		gEngfuncs.pTriAPI->Fog ( g_fFogColor, g_fStartDist, g_fEndDist, bFog );
//	else
//		gEngfuncs.pTriAPI->Fog ( g_fFogColor, 10000, 10001, 0 );
}
