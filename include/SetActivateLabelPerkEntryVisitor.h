#pragma once

#include <limits>  // numeric_limits

#include "RE/BGSPerkEntry.h"  // BGSPerkEntry
#include "RE/PerkEntryVisitor.h"  // PerkEntryVisitor
#include "RE/TESObjectREFR.h"  // TESObjectREFR


class SetActivateLabelPerkEntryVisitor : public RE::PerkEntryVisitor
{
public:
	constexpr SetActivateLabelPerkEntryVisitor(RE::TESObjectREFR* a_perkOwner, RE::TESObjectREFR* a_target) :
		_args{ a_perkOwner, a_target },
		_priority(std::numeric_limits<SInt16>::min())
	{}

	virtual ReturnType Visit(RE::BGSPerkEntry* a_perkEntry) override;

protected:
	enum
	{
		kNumArgs = 2
	};


	void*	_args[kNumArgs];
	SInt16	_priority;
};
