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
		HasACTITextOverrideVisitor(RE::Actor* a_perkOwner, RE::TESObjectREFR* a_target);

		virtual ReturnType Visit(RE::BGSPerkEntry* a_perkEntry) override;

	protected:
		RE::Actor*			_perkOwner;
		RE::TESObjectREFR*	_target;
	};
}
