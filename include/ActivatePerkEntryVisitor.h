#pragma once

#include "RE/Skyrim.h"


class ActivatePerkEntryVisitor : public RE::PerkEntryVisitor
{
public:
	constexpr ActivatePerkEntryVisitor(RE::TESObjectREFR* a_perkOwner, RE::TESObjectREFR* a_target) :
		_args{ a_perkOwner, a_target },
		_result(false)
	{}

	virtual ReturnType	Visit(RE::BGSPerkEntry* a_perkEntry) override;
	bool				GetResult() const;

private:
	enum
	{
		kNumArgs = 2
	};


	void*	_args[kNumArgs];
	bool	_result;
};
