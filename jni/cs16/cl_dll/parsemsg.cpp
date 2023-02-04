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
//  parsemsg.cpp
//

#include "parsemsg.h"

typedef unsigned char byte;
#define true 1

static byte *gpBuf;
static int giSize;
static int giRead;
static int giBadRead;

void BEGIN_READ( void *buf, int size )
{
	giRead = 0;
	giBadRead = 0;
	giSize = size;
	gpBuf = (byte*)buf;
}

int READ_BYTE( void )
{
	int c;

	if( giRead + 1 > giSize )
	{
		giBadRead = true;
		return -1;
	}
		
	c = (unsigned char)gpBuf[giRead];
	giRead++;
	
	return c;
}

int READ_SHORT( void )
{
	int c;

	if( giRead + 2 > giSize )
	{
		giBadRead = true;
		return -1;
	}

	c = (short)( gpBuf[giRead] + ( gpBuf[giRead + 1] << 8 ) );

	giRead += 2;

	return c;
}
