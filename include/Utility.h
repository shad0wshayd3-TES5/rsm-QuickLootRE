#pragma once

#include <string>  // string
#include <type_traits>  // underlying_type_t

namespace RE
{
	class TESObjectREFR;
}


namespace QuickLootRE
{
	std::string boolToString(bool a_bool);
	std::string	numToHexString(UInt64 a_num, UInt64 a_bytes = 4);
	bool		IsValidPickPocketTarget(RE::TESObjectREFR* a_refr, bool a_isSneaking);
	template <typename EnumType>
	constexpr auto to_underlying(EnumType a_typedEnum) noexcept
	{
		return static_cast<std::underlying_type_t<EnumType>>(a_typedEnum);
	}
}
