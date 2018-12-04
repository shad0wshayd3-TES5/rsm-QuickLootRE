#include "Utility.h"

#include <cctype>  // toupper
#include <ios>  // hex
#include <sstream>  // stringstream
#include <string>  // string, strlen

#include "Forms.h"  // KYWDFormID

#include "RE/Actor.h"  // Actor
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESFaction.h"  // TESFaction
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/TESRace.h"  // TESRace


namespace QuickLootRE
{
	std::string boolToString(bool a_bool)
	{
		return a_bool ? "True" : "False";
	}


	std::string numToHexString(UInt64 a_num, UInt64 a_bytes)
	{
		// Convert to hex
		std::stringstream sstream;
		sstream << std::hex << a_num;
		std::string hexStr = sstream.str();

		// Convert lowercase characters to uppercase
		for (auto& c : hexStr) {
			c = toupper(c);
		}

		// Sign extension
		while (std::strlen(hexStr.c_str()) < a_bytes * 2) {
			hexStr = '0' + hexStr;
		}

		return hexStr;
	}


	bool IsValidPickPocketTarget(RE::TESObjectREFR* a_refr, bool a_isSneaking)
	{
		if (!a_refr || a_refr->baseForm->formType != RE::FormType::NPC) {
			return false;
		}

		RE::Actor* actor = static_cast<RE::Actor*>(a_refr);
		if (!actor) {
			return false;
		}

		RE::TESRace* race = actor->GetRace();
		if (!race) {
			return false;
		}

		return (a_isSneaking &&
				!actor->IsDead(true) &&
				!actor->IsGhost() &&
				!actor->IsChild() &&
				!actor->IsPlayerTeammate() &&
				!actor->IsInFaction(CurrentFollowerFaction) &&
				!actor->GetRace()->HasKeyword(ActorTypeCreature) &&
				!actor->GetRace()->HasKeyword(ActorTypeAnimal));
	}
}
