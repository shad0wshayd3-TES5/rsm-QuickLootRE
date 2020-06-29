#pragma once

#include "CLIK/Object.h"

class GFxItem
{
public:
	inline GFxItem(observer<RE::InventoryEntryData*> a_item) :
		_displayName(a_item->GetDisplayName())
	{}

	inline GFxItem(observer<RE::TESObjectREFR*> a_item) :
		_displayName()
	{
		const auto name = a_item->GetDisplayFullName();
		if (name) {
			_displayName = name;
		}
	}

	[[nodiscard]] inline CLIK::Object Object() const
	{
		return CLIK::Object{ _displayName };
	}

private:
	std::string _displayName;
};
