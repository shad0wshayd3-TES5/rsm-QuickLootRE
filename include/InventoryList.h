#pragma once

#include <map>  // map
#include <vector>  // vector

#include "ItemData.h"  // ItemData
#include "ManagedEntryData.h"  // ManagedEntryDataPtr

#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/TESContainer.h"  // TESContainer::Entry
#include "RE/TESForm.h"  // TESForm
#include "RE/TESObjectREFR.h"  // TESObjectREFR


class InventoryList
{
public:
	struct EntryDataCountPair
	{
		EntryDataCountPair(ManagedEntryDataPtr a_entryData, SInt32 a_count);


		ManagedEntryDataPtr	entryData;
		SInt32				count;
	};


	using FormID = UInt32;
	using DefaultMap = std::map<FormID, EntryDataCountPair>;
	using ItemList = std::vector<ItemData>;


	static InventoryList&	GetSingleton();

	void				parseInventory(RE::TESObjectREFR* a_refr);
	ItemData&			operator[](UInt32 a_pos);
	ItemList::iterator	begin() noexcept;
	ItemList::iterator	end() noexcept;
	bool				empty() const noexcept;
	UInt32				size() const noexcept;
	void				clear() noexcept;
	ItemList::iterator	erase(ItemList::iterator a_pos);

private:
	InventoryList();
	~InventoryList();

	void	add(ManagedEntryDataPtr& a_entryData);
	void	add(ManagedEntryDataPtr& a_entryData, SInt32 a_count);
	void	parseInventoryChanges(RE::TESObjectREFR* a_refr);
	void	parseDroppedList(RE::TESObjectREFR* a_refr);
	bool	isValidItem(RE::TESForm* a_item);


	static InventoryList	_singleton;
	DefaultMap				_defaultMap;
	std::vector<ItemData>	_itemList;


	class TESContainerVisitor
	{
	public:
		TESContainerVisitor() = delete;
		explicit TESContainerVisitor(DefaultMap& a_defaultMap);

		virtual bool Accept(RE::TESContainer::Entry* a_entry);

	private:
		DefaultMap& _defaultMap;
	};
};
