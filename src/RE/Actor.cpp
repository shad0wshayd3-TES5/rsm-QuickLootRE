#include "RE/Actor.h"

#include "skse64/GameForms.h"  // TESRace

#include "RE/TESNPC.h"  // TESNPC


namespace RE
{
	TESRace * Actor::GetRace()
	{
		TESRace* outRace = 0;

		TESNPC* actorBase = GetActorBase();
		if (actorBase) {
			outRace = actorBase->race.race;
		}

		return outRace;
	}


	bool Actor::IsSneaking()
	{
		if (!actorState.IsSneaking()) {
			return false;
		}

		if (actorState.IsSwimming()) {
			return false;
		}

		if (IsOnMount()) {
			return false;
		}

		return true;
	}
}
