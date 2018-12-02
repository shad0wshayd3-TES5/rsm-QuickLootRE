#pragma once

#include "RE/PerkEntryVisitor.h"  // PerkEntryVisitor

namespace RE
{
	class BGSPerkEntry;
	class TESObjectREFR;
}


namespace QuickLootRE
{
	class ActivatePerkEntryVisitor : public RE::PerkEntryVisitor
	{
		typedef PerkEntryVisitor::ReturnType ReturnType;
	public:
		constexpr ActivatePerkEntryVisitor(RE::TESObjectREFR* a_perkOwner, RE::TESObjectREFR* a_target) :
			_perkOwner(a_perkOwner),
			_target(a_target),
			_result(false)
		{}

		virtual ReturnType	Visit(RE::BGSPerkEntry* a_perkEntry) override;
		bool				GetResult() const;

	private:
		RE::TESObjectREFR*	_perkOwner;
		RE::TESObjectREFR*	_target;
		bool				_result;
	};
}
