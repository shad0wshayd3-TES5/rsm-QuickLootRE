#pragma once

#include "CLIK/Object.h"
#include "GFxItem.h"

class InventoryItem
{
public:
	InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_entry, std::ptrdiff_t a_count);
	InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_entry, std::ptrdiff_t a_count, RE::ObjectRefHandle a_container);
	InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_entry, std::ptrdiff_t a_count, RE::TESObjectREFRPtr a_container);

	[[nodiscard]] std::ptrdiff_t Count() const;
	[[nodiscard]] CLIK::Object Object() const;

	void Take(RE::TESObjectREFRPtr a_dst, std::ptrdiff_t a_count);
	void Take(RE::TESObjectREFRPtr a_dst);
	void TakeAll(RE::TESObjectREFRPtr a_dst);

private:
	std::unique_ptr<RE::InventoryEntryData> _entry;
	RE::ObjectRefHandle _container;
	std::ptrdiff_t _count;
	GFxItem _gfxItem;
};
