#include "ActivatePerkEntryVisitor.h"

#include "RE/BGSEntryPointFunctionDataActivateChoice.h"  // BGSEntryPointFunctionDataActivateChoice
#include "RE/BGSEntryPointPerkEntry.h"  // BGSEntryPointPerkEntry
#include "RE/BGSPerkEntry.h"  // BGSPerkEntry
#include "RE/Condition.h"  // Condition


ActivatePerkEntryVisitor::ReturnType ActivatePerkEntryVisitor::Visit(RE::BGSPerkEntry* a_perkEntry)
{
	using EntryPointType = RE::BGSEntryPointPerkEntry::EntryPointType;

	RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(a_perkEntry);
	if (entryPoint && entryPoint->IsType(EntryPointType::kActivate)) {
		if (entryPoint->EvaluateConditions(kNumArgs, _args)) {
			if (entryPoint->functionData) {
				RE::BGSEntryPointFunctionDataActivateChoice* fnDataActivateChoice = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);
				if (fnDataActivateChoice->ReplacesDefault() && fnDataActivateChoice->RunsImmediately()) {
					_result = true;
					return ReturnType::kBreak;
				}
			}
		}
	}

	return ReturnType::kContinue;
}


bool ActivatePerkEntryVisitor::GetResult() const
{
	return _result;
}
