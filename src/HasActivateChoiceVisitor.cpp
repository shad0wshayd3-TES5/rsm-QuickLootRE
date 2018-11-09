#include "HasActivateChoiceVisitor.h"

#include "skse64/GameForms.h"  // BGSPerk

#include "Forms.h"  // PERKFormID

#include "RE/BGSEntryPointFunctionDataActivateChoice.h"  // RE::BGSEntryPointFunctionDataActivateChoice
#include "RE/BGSEntryPointPerkEntry.h"  // RE::BGSEntryPointPerkEntry
#include "RE/BGSPerkEntry.h"  // RE::BGSPerkEntry

namespace RE
{
	class Actor;
	class TESObjectREFR;
}



namespace QuickLootRE
{
	HasActivateChoiceVisitor::HasActivateChoiceVisitor(RE::Actor* a_actor, RE::TESObjectREFR* a_target) :
		_actor(a_actor),
		_target(a_target),
		_result(false)
	{}


	UInt32 HasActivateChoiceVisitor::Visit(RE::BGSPerkEntry* perkEntry)
	{
		if (perkEntry->CanProcess(2, &_actor)) {
			RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(perkEntry);
			if (entryPoint->functionData) {
				_DMESSAGE("[DEBUG] 0x%p", entryPoint);
				RE::BGSEntryPointFunctionDataActivateChoice* activateChoice = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);
				if (entryPoint->perk && activateChoice) {
					if ((activateChoice->flags & RE::BGSEntryPointFunctionDataActivateChoice::kFlag_RunImmediately) == 0) {
						switch (entryPoint->perk->formID) {
						case kPERKFormID_VampireFeed:
						case kPERKFormID_Cannibalism:
							break;
						default:
							_result = true;
						}
					}
					if ((activateChoice->flags & RE::BGSEntryPointFunctionDataActivateChoice::kFlag_ReplaceDefault) != 0) {
						_result = true;
					}
				}
			}
		}
		return 1;
	}
}
