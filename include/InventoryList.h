#pragma once

#include <utility>  // pair
#include <map>  // map
#include <vector>  // vector

#include "ItemData.h"  // ItemData

#include "RE/TESContainer.h"  // TESContainer::Entry

namespace RE
{
	class InventoryEntryData;
	class TESForm;
	class TESObjectREFR;
}


namespace QuickLootRE
{
	class InventoryList
	{
	public:
		InventoryList();
		~InventoryList();

		void							parseInventory(RE::TESObjectREFR* a_refr);

		ItemData&						operator[](UInt32 a_pos);

		std::vector<ItemData>::iterator	begin() noexcept;
		std::vector<ItemData>::iterator	end() noexcept;

		bool							empty();
		UInt32							size();

		void							clear();
		std::vector<ItemData>::iterator	erase(std::vector<ItemData>::iterator a_pos);

	private:
		void							add(RE::InventoryEntryData* a_entryData);
		void							add(RE::InventoryEntryData* a_entryData, SInt32 a_count);

		void							parseInventoryChanges(RE::TESObjectREFR* a_refr);
		void							parseDroppedList(RE::TESObjectREFR* a_refr);

		bool							isValidItem(RE::TESForm* a_item);

		typedef UInt32 FormID;
		typedef SInt32 Count;
		std::map<FormID, std::pair<RE::InventoryEntryData*, Count>>	_defaultMap;
		std::vector<ItemData>										_itemList;
		std::vector<RE::InventoryEntryData*>						_heapList;


		class TESContainerVisitor
		{
		public:
			explicit TESContainerVisitor(std::map<FormID, std::pair<RE::InventoryEntryData*, Count>>& a_defaultMap, std::vector<RE::InventoryEntryData*>& a_heapList);

			virtual bool Accept(RE::TESContainer::Entry* a_entry);

		private:
			std::map<FormID, std::pair<RE::InventoryEntryData*, Count>>&	_defaultMap;
			std::vector<RE::InventoryEntryData*>							_heapList;
		};
	};


	extern InventoryList g_invList;
}
