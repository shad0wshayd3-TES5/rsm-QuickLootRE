#include "GFxItem.h"

GFxItem::GFxItem(const std::unique_ptr<RE::InventoryEntryData>& a_entry) :
	_displayName(a_entry->GetDisplayName())
{}

CLIK::Object GFxItem::Object() const
{
	CLIK::Object object;
	object = _displayName;
	return object;
}
