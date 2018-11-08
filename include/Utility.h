#pragma once

#include <string>  // string

namespace RE
{
	class TESObjectREFR;
}

namespace QuickLootRE
{
	std::string boolToString(bool a_bool);
	std::string	numToHexString(UInt64 a_num, UInt64 a_bytes = 4);
	bool		IsValidPickPocketTarget(RE::TESObjectREFR* a_refr, bool a_isSneaking);
}
