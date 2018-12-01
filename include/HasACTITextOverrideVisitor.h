#pragma once

#include "RE/PerkEntryVisitor.h"  // RE::PerkEntryVisitor

namespace RE
{
	class Actor;
	class BGSPerkEntry;
	class TESObjectREFR;
}


namespace QuickLootRE
{
	class HasACTITextOverrideVisitor : public RE::PerkEntryVisitor
	{
	public:
		constexpr HasACTITextOverrideVisitor(RE::TESObjectREFR* a_perkOwner, RE::TESObjectREFR* a_target) :
			_perkOwner(a_perkOwner),
			_target(a_target)
		{}

		virtual ReturnType Visit(RE::BGSPerkEntry* a_perkEntry) override;

	protected:
		RE::TESObjectREFR*	_perkOwner;
		RE::TESObjectREFR*	_target;
	};
}
