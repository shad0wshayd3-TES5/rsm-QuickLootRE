#include "Utility.h"

#include "Forms.h"


bool IsValidPickPocketTarget(RE::TESObjectREFR* a_ref, bool a_isSneaking)
{
	auto baseObj = a_ref ? a_ref->GetBaseObject() : 0;
	if (!baseObj || baseObj->IsNot(RE::FormType::NPC)) {
		return false;
	}

	auto actor = static_cast<RE::Actor*>(a_ref);
	auto race = actor->GetRace();
	if (!race) {
		return false;
	}

	return (a_isSneaking &&
			!actor->IsDead() &&
			!actor->IsGhost() &&
			!actor->IsChild() &&
			!actor->IsPlayerTeammate() &&
			race->AllowsPickpocket());
}
