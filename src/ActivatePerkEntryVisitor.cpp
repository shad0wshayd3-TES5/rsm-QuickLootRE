#include "ActivatePerkEntryVisitor.h"

#include "RE/BGSEntryPointFunctionDataActivateChoice.h"  // BGSEntryPointFunctionDataActivateChoice
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
	ActivatePerkEntryVisitor::ReturnType ActivatePerkEntryVisitor::Visit(RE::BGSPerkEntry* a_perkEntry)
	{
		typedef RE::BGSEntryPointPerkEntry::EntryPointType EntryPointType;

		RE::BGSEntryPointPerkEntry* entryPoint = static_cast<RE::BGSEntryPointPerkEntry*>(a_perkEntry);
		if (entryPoint && entryPoint->HasType(EntryPointType::kActivate)) {
			if (entryPoint->conditions && entryPoint->conditions->Run(_perkOwner, _target)) {
				if (entryPoint->functionData) {
					RE::BGSEntryPointFunctionDataActivateChoice* fnDataActivateChoice = static_cast<RE::BGSEntryPointFunctionDataActivateChoice*>(entryPoint->functionData);
					if (fnDataActivateChoice->ReplacesDefault() && fnDataActivateChoice->RunsImmediately()) {
						_result = true;
						return ReturnType::kReturnType_Break;
					}
				}
			}
		}

		return ReturnType::kReturnType_Continue;
	}


	bool ActivatePerkEntryVisitor::GetResult() const
	{
		return _result;
	}
}
