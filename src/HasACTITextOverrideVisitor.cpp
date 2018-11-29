#include "HasACTITextOverrideVisitor.h"

#include "skse64/GameForms.h"  // BGSPerk

#include "LootMenu.h"

#include "RE/Actor.h"  // Actor
#include "RE/BGSEntryPointFunctionDataText.h"  // BGSEntryPointFunctionDataText
#include "RE/BGSEntryPointPerkEntry.h"  // BGSEntryPointPerkEntry
#include "RE/BGSPerkEntry.h"  // BGSPerkEntry
#include "RE/Condition.h"  // Condition

namespace RE
{
	class Actor;
	class TESObjectREFR;
}



namespace QuickLootRE
{
	HasACTITextOverrideVisitor::HasACTITextOverrideVisitor(RE::Actor* a_perkOwner, RE::TESObjectREFR* a_target) :
		_perkOwner(a_perkOwner),
		_target(a_target)
	{}


	HasACTITextOverrideVisitor::ReturnType HasACTITextOverrideVisitor::Visit(RE::BGSPerkEntry* perkEntry)
	{
		typedef RE::BGSEntryPointPerkEntry::EntryPointType EntryPointType;

		RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(perkEntry);
		if (entryPoint && entryPoint->HasType(EntryPointType::kEntryPoint_Set_Activate_Label)) {
			if (entryPoint->conditions && entryPoint->conditions->Run(_perkOwner, _target)) {
				RE::BGSEntryPointFunctionDataText* fnDataText = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);
				if (fnDataText) {
					LootMenu::SetActiText(fnDataText->text);
				}
			}
		}

		return ReturnType::kReturnType_Continue;
	}
}
