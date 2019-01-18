#pragma once

#include <string>  // string

namespace RE
{
	class TESObjectREFR;
}


namespace QuickLootRE
{
	std::string boolToString(bool a_bool);
	bool		IsValidPickPocketTarget(RE::TESObjectREFR* a_refr, bool a_isSneaking);
}
