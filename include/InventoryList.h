#pragma once

#include <map>
#include <vector>

#include "ItemData.h"
#include "ManagedEntryData.h"

#include "RE/Skyrim.h"


class InventoryList
{
public:
	InventoryList() = default;
	~InventoryList() = default;


	struct EntryDataCountPair
	{
		EntryDataCountPair(ManagedEntryDataPtr a_entryData, SInt32 a_count);


		ManagedEntryDataPtr	entryData;
		SInt32				count;
	};


	using FormID = UInt32;
	using DefaultMap = std::map<FormID, EntryDataCountPair>;
	using ItemList = std::vector<ItemData>;


	void				parseInventory(RE::TESObjectREFR* a_refr);
	ItemData&			operator[](UInt32 a_pos);
	ItemList::iterator	begin() noexcept;
	ItemList::iterator	end() noexcept;
	bool				empty() const noexcept;
	std::size_t			size() const noexcept;
	void				clear() noexcept;
	ItemList::iterator	erase(ItemList::iterator a_pos);

private:
	void	add(ManagedEntryDataPtr& a_entryData);
	void	add(ManagedEntryDataPtr& a_entryData, SInt32 a_count);
	void	parseInventoryChanges(RE::TESObjectREFR* a_refr);
	void	parseDroppedList(RE::TESObjectREFR* a_refr);
	bool	isValidItem(RE::TESForm* a_item);


	DefaultMap	_defaultMap;
	ItemList	_itemList;
};
