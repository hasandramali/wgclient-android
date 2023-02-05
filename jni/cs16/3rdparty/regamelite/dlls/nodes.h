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

#ifndef NODES_H
#define NODES_H
#ifdef _WIN32
#pragma once
#endif

#define bits_NODE_LAND		(1 << 0)	// Land node, so nudge if necessary.
#define bits_NODE_AIR		(1 << 1)	// Air node, don't nudge.
#define bits_NODE_WATER		(1 << 2)	// Water node, don't nudge.
#define bits_NODE_GROUP_REALM	(bits_NODE_LAND | bits_NODE_AIR | bits_NODE_WATER)

class CBaseEntity;

class CLink
{
public:
	entvars_t *m_pLinkEnt;
};

class CGraph
{
public:
	void InitGraph();
	int AllocNodes();
	int CheckNODFile(char *szMapName);
	int FLoadGraph(char *szMapName);
	int FSetGraphPointers();
	void ShowNodeConnections(int iNode);
	int FindNearestNode(const Vector &vecOrigin, CBaseEntity *pEntity);
	int FindNearestNode(const Vector &vecOrigin, int afNodeTypes);

public:
	BOOL m_fGraphPresent;
	BOOL m_fGraphPointersSet;
	int m_cLinks;
	CLink *m_pLinkPool;
};

extern CGraph WorldGraph;

#endif // NODES_H
