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
	class HaACTITextOverrideVisitor : public RE::PerkEntryVisitor
	{
	public:
		HaACTITextOverrideVisitor(RE::Actor* a_actor, RE::TESObjectREFR* a_target);

		virtual UInt32 Visit(RE::BGSPerkEntry* perkEntry) override;

	protected:
		RE::Actor*			_actor;
		RE::TESObjectREFR*	_target;
	};
}
