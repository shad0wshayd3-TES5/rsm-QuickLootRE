#include "HasACTITextOverrideVisitor.h"

#include "skse64/GameForms.h"  // BGSPerk

#include "LootMenu.h"

#include "RE/BGSEntryPointFunctionDataText.h"  // BGSEntryPointFunctionDataText
#include "RE/BGSEntryPointPerkEntry.h"  // BGSEntryPointPerkEntry
#include "RE/BGSPerkEntry.h"  // BGSPerkEntry

namespace RE
{
	class Actor;
	class TESObjectREFR;
}



namespace QuickLootRE
{
	HasACTITextOverrideVisitor::HasACTITextOverrideVisitor(RE::Actor* a_actor, RE::TESObjectREFR* a_target) :
		_actor(a_actor),
		_target(a_target)
	{}


	UInt32 HasACTITextOverrideVisitor::Visit(RE::BGSPerkEntry* perkEntry)
	{
		typedef RE::BGSEntryPointPerkEntry::EntryPointType EntryPointType;

		if (perkEntry->CanProcess(2, &_actor)) {
			RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(perkEntry);
			if (entryPoint->HasType(EntryPointType::kEntryPoint_Set_Activate_Label)) {
				RE::BGSEntryPointFunctionDataText* fnDataText = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);
				if (fnDataText) {
					_DMESSAGE("[DEBUG] 0x%p", fnDataText);
					LootMenu::SetActiText(fnDataText->text);
				}
			}
		}
		return 1;
	}
}
