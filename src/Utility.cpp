#include "Utility.h"

#include "Forms.h"

#include "RE/Skyrim.h"


std::string boolToString(bool a_bool)
{
	return a_bool ? "True" : "False";
}


bool IsValidPickPocketTarget(RE::TESObjectREFR* a_refr, bool a_isSneaking)
{
	if (!a_refr || a_refr->baseForm->IsNot(RE::FormType::NPC)) {
		return false;
	}

	auto actor = static_cast<RE::Actor*>(a_refr);
	if (!actor) {
		return false;
	}

	auto race = actor->GetRace();
	if (!race) {
		return false;
	}

	return (a_isSneaking &&
			!actor->IsDead(true) &&
			!actor->IsGhost() &&
			!actor->IsChild() &&
			!actor->IsPlayerTeammate() &&
			!actor->IsInFaction(CurrentFollowerFaction) &&
			race->AllowsPickpocket());
}
