// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"

void EscapeFromBombState::__MAKE_VHOOK(OnEnter)(CCSBot *me)
{
	me->StandUp();
	me->Run();
	me->DestroyPath();
	me->EquipKnife();
}

// Escape from the bomb
void EscapeFromBombState::__MAKE_VHOOK(OnUpdate)(CCSBot *me)
{
	const Vector *bombPos = me->GetGameState()->GetBombPosition();

	// if we don't know where the bomb is, we shouldn't be in this state
	if (bombPos == NULL)
	{
		me->Idle();
		return;
	}

	// grab our knife to move quickly
	me->EquipKnife();

	// look around
	me->UpdateLookAround();

	if (me->UpdatePathMovement() != CCSBot::PROGRESSING)
	{
		// we have no path, or reached the end of one - create a new path far away from the bomb
		FarAwayFromPositionFunctor func(bombPos);
		CNavArea *goalArea = FindMinimumCostArea(me->GetLastKnownArea(), func);

		// if this fails, we'll try again next time
		me->ComputePath(goalArea, NULL, FASTEST_ROUTE);
	}
}

void EscapeFromBombState::__MAKE_VHOOK(OnExit)(CCSBot *me)
{
	me->EquipBestWeapon();
}
