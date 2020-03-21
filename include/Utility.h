#pragma once

#include "RE/Skyrim.h"


enum Style : UInt32
{
	kDefault = 0,
	kDialogue = 1
};


constexpr const char* boolToString(bool a_bool)
{
	return a_bool ? "True" : "False";
}


bool IsValidPickPocketTarget(RE::TESObjectREFR* a_ref, bool a_isSneaking);
