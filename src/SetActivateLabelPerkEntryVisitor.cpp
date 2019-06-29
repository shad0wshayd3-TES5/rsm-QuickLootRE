#include "SetActivateLabelPerkEntryVisitor.h"

#include "LootMenu.h"

#include "RE/Skyrim.h"


auto SetActivateLabelPerkEntryVisitor::Visit(RE::BGSPerkEntry* a_perkEntry)
-> ReturnType
{
	using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;

	auto entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(a_perkEntry);
	if (entryPoint && entryPoint->IsEntryPoint(EntryPoint::kSetActivateLabel)) {
		if (entryPoint->EvaluateConditions(kNumArgs, _args)) {
			auto fnDataText = static_cast<RE::BGSEntryPointFunctionDataText*>(entryPoint->functionData);
			if (fnDataText && a_perkEntry->GetPriority() > _priority) {
				auto loot = LootMenu::GetSingleton();
				loot->SetActiText(fnDataText->text.c_str());
				_priority = a_perkEntry->GetPriority();
			}
		}
	}

	return ReturnType::kContinue;
}
