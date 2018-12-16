#pragma once

#undef min
#undef max

#include <limits>  // numeric_limits

#include "RE/PerkEntryVisitor.h"  // PerkEntryVisitor

namespace RE
{
	class Actor;
	class BGSPerkEntry;
	class TESObjectREFR;
}


namespace QuickLootRE
{
	class SetActivateLabelPerkEntryVisitor : public RE::PerkEntryVisitor
	{
	public:
		constexpr SetActivateLabelPerkEntryVisitor(RE::TESObjectREFR* a_perkOwner, RE::TESObjectREFR* a_target) :
			_perkOwner(a_perkOwner),
			_target(a_target),
			_priority(std::numeric_limits<SInt16>::min())
		{}

		virtual ReturnType Visit(RE::BGSPerkEntry* a_perkEntry) override;

	protected:
		RE::TESObjectREFR*	_perkOwner;
		RE::TESObjectREFR*	_target;
		SInt16				_priority;
	};
}
