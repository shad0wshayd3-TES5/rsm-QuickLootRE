#include "ActivatePerkEntryVisitor.h"

#include "RE/Skyrim.h"


auto ActivatePerkEntryVisitor::Visit(RE::BGSPerkEntry* a_perkEntry)
-> ReturnType
{
	using EntryPoint = RE::BGSEntryPointPerkEntry::EntryPoint;

	RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(a_perkEntry);
	if (entryPoint && entryPoint->IsEntryPoint(EntryPoint::kActivate)) {
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
