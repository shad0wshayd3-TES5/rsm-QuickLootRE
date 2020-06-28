#pragma once

#include "CLIK/Object.h"

class GFxItem
{
public:
	GFxItem(const std::unique_ptr<RE::InventoryEntryData>& a_entry);

	[[nodiscard]] CLIK::Object Object() const;

private:
	std::string _displayName;
};
