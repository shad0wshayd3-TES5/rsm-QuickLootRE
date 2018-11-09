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
	class HasActivateChoiceVisitor : public RE::PerkEntryVisitor
	{
	public:
		HasActivateChoiceVisitor(RE::Actor* a_actor, RE::TESObjectREFR* a_target);

		virtual UInt32	Visit(RE::BGSPerkEntry* perkEntry) override;
		inline bool		GetResult() const { return _result; }

	protected:
		RE::Actor*			_actor;
		RE::TESObjectREFR*	_target;
		bool				_result;
	};
}
