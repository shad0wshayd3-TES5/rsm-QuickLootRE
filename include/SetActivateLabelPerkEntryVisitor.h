#pragma once

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
			_target(a_target)
		{}

		virtual ReturnType Visit(RE::BGSPerkEntry* a_perkEntry) override;

	protected:
		RE::TESObjectREFR*	_perkOwner;
		RE::TESObjectREFR*	_target;
	};
}
