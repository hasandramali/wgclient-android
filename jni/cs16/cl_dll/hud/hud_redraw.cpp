/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// hud_redraw.cpp
//
#include <math.h>
#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"

#include <string.h>
#define MAX_LOGO_FRAMES 56

int grgLogoFrame[MAX_LOGO_FRAMES] = 
{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 13, 13, 13, 13, 13, 12, 11, 10, 9, 8, 14, 15,
	16, 17, 18, 19, 20, 20, 20, 20, 20, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
	29, 29, 29, 29, 29, 28, 27, 26, 25, 24, 30, 31 
};


extern int g_iVisibleMouse;

float HUD_GetFOV( void );

// Think
void CHud::Think(void)
{
	m_Rainbow.Think( );
	int newfov;

	for( HUDLIST *pList = m_pHudList; pList; pList = pList->pNext )
	{
		if( pList->p->m_iFlags & HUD_THINK )
			pList->p->Think();
	}

	newfov = HUD_GetFOV();
	m_iFOV = newfov ? newfov : default_fov->value;

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if ( m_iFOV == default_fov->value )
	{  
		// reset to saved sensitivity
		m_flMouseSensitivity = 0;
	}
	else
	{  
		// set a new sensitivity that is proportional to the change from the FOV default
		m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)default_fov->value) * zoom_sens_ratio->value;
	}

	// think about default fov
	if ( m_iFOV == 0 )
	{  // only let players adjust up in fov,  and only if they are not overriden by something else
		m_iFOV = max( default_fov->value, 90 );
	}

}

// Fog
extern float g_fFadeDuration;
extern float g_fStartDist;
extern float g_fEndDist;
//extern int g_iFinalStartDist;
extern int g_iFinalEndDist;

// Redraw
// step through the local data,  placing the appropriate graphics & text as appropriate
// returns 1 if they've changed, 0 otherwise
int CHud :: Redraw( float flTime, int intermission )
{
	m_fOldTime = m_flTime;	// save time of previous redraw
	m_flTime = flTime;
	m_flTimeDelta = (double)m_flTime - m_fOldTime;
	static int m_flShotTime = 0;

	//LRC - handle fog fading effects. (is this the right place for it?)
	if (g_fFadeDuration)
	{
		// Nicer might be to use some kind of logarithmic fade-in?
		double fFraction = m_flTimeDelta/g_fFadeDuration;
//		g_fStartDist -= (FOG_LIMIT - g_iFinalStartDist)*fFraction;
		g_fEndDist -= (FOG_LIMIT - g_iFinalEndDist)*fFraction;

//		CONPRINT("FogFading: %f - %f, frac %f, time %f, final %d\n", g_fStartDist, g_fEndDist, fFraction, flTime, g_iFinalEndDist);

		// cap it
//		if (g_fStartDist > FOG_LIMIT)				g_fStartDist = FOG_LIMIT;
		if (g_fEndDist   > FOG_LIMIT)				g_fEndDist = FOG_LIMIT;
//		if (g_fStartDist < g_iFinalStartDist)	g_fStartDist = g_iFinalStartDist;
		if (g_fEndDist   < g_iFinalEndDist)		g_fEndDist   = g_iFinalEndDist;
	}

#ifdef __ANDROID__
	if( cl_android_force_defaults && cl_android_force_defaults->value )
	{
		if( cl_lw && !cl_lw->value )
		{
			ConsolePrint( "cl_lw is forced to 1. Set cl_android_force_defaults to 0, if you want to disable this behaviour" );
			gEngfuncs.Cvar_SetValue( "cl_lw", 1.0f );
		}
		if( cl_predict && !cl_predict->value )
		{
			ConsolePrint( "cl_predict is forced to 1. Set cl_android_force_defaults to 0, if you want to disable this behaviour" );
			gEngfuncs.Cvar_SetValue( "cl_predict", 1.0f );
		}
		if( sv_skipshield && !sv_skipshield->value )
		{
			ConsolePrint( "sv_skipshield is forced to 1. Set cl_android_force_defaults to 0, if you want to disable this behaviour" );
			gEngfuncs.Cvar_SetValue( "sv_skipshield", 1.0f );
		}
	}
#endif
	
	// Clock was reset, reset delta
	if ( m_flTimeDelta < 0 )
		m_flTimeDelta = 0;

	if (m_flShotTime && m_flShotTime < flTime)
	{
		gEngfuncs.pfnClientCmd("snapshot\n");
		m_flShotTime = 0;
	}

	m_iIntermission = intermission;

	if ( m_pCvarDraw->value && (intermission || !(m_iHideHUDDisplay & HIDEHUD_ALL) ) )
	{
		for( HUDLIST *pList = m_pHudList; pList; pList = pList->pNext )
		{
			if( pList->p->m_iFlags & HUD_DRAW )
			{
				if( intermission && !(pList->p->m_iFlags & HUD_INTERMISSION) )
					continue; // skip no-intermission during intermission

				pList->p->Draw( flTime );
			}
		}
		UpdateDefaultHUDColor( );
	}

	// are we in demo mode? do we need to draw the logo in the top corner?
	if (m_iLogo)
	{
		int x, y, i;

		if (m_hsprLogo == 0)
			m_hsprLogo = LoadSprite("sprites/%d_logo.spr");

		SPR_Set(m_hsprLogo, 250, 250, 250 );
		
		x = SPR_Width(m_hsprLogo, 0);
		x = ScreenWidth - x;
		y = SPR_Height(m_hsprLogo, 0)/2;

		// Draw the logo at 20 fps
		int iFrame = (int)(flTime * 20) % MAX_LOGO_FRAMES;
		i = grgLogoFrame[iFrame] - 1;

		SPR_DrawAdditive(i, x, y, NULL);
	}

	return 1;
}
