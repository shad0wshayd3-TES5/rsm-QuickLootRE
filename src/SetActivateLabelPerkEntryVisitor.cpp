#include "SetActivateLabelPerkEntryVisitor.h"

#include "LootMenu.h"

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
	SetActivateLabelPerkEntryVisitor::ReturnType SetActivateLabelPerkEntryVisitor::Visit(RE::BGSPerkEntry* a_perkEntry)
	{
		typedef RE::BGSEntryPointPerkEntry::EntryPointType EntryPointType;

		RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(a_perkEntry);
		if (entryPoint && entryPoint->HasType(EntryPointType::kEntryPoint_Set_Activate_Label)) {
			if (entryPoint->conditions && entryPoint->conditions->Run(_perkOwner, _target)) {
				RE::BGSEntryPointFunctionDataText* fnDataText = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);
				if (fnDataText && a_perkEntry->priority > _priority) {
					LootMenu::SetActiText(fnDataText->text);
					_priority = a_perkEntry->priority;
				}
			}
		}

		return ReturnType::kReturnType_Continue;
	}
}
