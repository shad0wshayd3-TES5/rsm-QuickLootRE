#include "SetActivateLabelPerkEntryVisitor.h"

#include "LootMenu.h"  // LootMenu

#include "RE/BGSEntryPointFunctionDataText.h"  // BGSEntryPointFunctionDataText
#include "RE/BGSEntryPointPerkEntry.h"  // BGSEntryPointPerkEntry
#include "RE/BGSPerkEntry.h"  // BGSPerkEntry


SetActivateLabelPerkEntryVisitor::ReturnType SetActivateLabelPerkEntryVisitor::Visit(RE::BGSPerkEntry* a_perkEntry)
{
	using EntryPointType = RE::BGSEntryPointPerkEntry::EntryPointType;

	RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(a_perkEntry);
	if (entryPoint && entryPoint->IsType(EntryPointType::kSetActivateLabel)) {
		if (entryPoint->EvaluateConditions(kNumArgs, _args)) {
			RE::BGSEntryPointFunctionDataText* fnDataText = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);
			if (fnDataText && a_perkEntry->priority > _priority) {
				LootMenu::GetSingleton()->SetActiText(fnDataText->text.c_str());
				_priority = a_perkEntry->priority;
			}
		}
	}

	return ReturnType::kContinue;
}
