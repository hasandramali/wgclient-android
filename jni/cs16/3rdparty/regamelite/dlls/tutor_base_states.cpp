// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

CBaseTutorStateSystem::CBaseTutorStateSystem()
{
	;
}

CBaseTutorStateSystem::~CBaseTutorStateSystem()
{
	;
}

int CBaseTutorStateSystem::GetCurrentStateType() const
{
	if (m_currentState)
	{
		return m_currentState->GetType();
	}

	return 0;
}

CBaseTutorState::CBaseTutorState()
{
	;
}

CBaseTutorState::~CBaseTutorState()
{
	;
}

int CBaseTutorState::GetType() const
{
	return m_type;
}
