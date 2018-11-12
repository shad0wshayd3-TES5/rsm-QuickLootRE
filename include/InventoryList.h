#pragma once

#include <utility>  // pair
#include <map>  // map
#include <vector>  // vector

#include "ItemData.h"  // ItemData

class TESContainer;
class TESForm;

namespace RE
{
	class BaseExtraList;
	class InventoryEntryData;
	class TESObjectREFR;
}


namespace QuickLootRE
{
	class InventoryList
	{
	public:
		InventoryList();
		~InventoryList();

		void											parseInventory(RE::BaseExtraList* a_xList, RE::TESObjectREFR* a_refr);
		void											stage(std::vector<ItemData>::iterator a_pos, SInt32 a_count);
		void											discard();
		void											commit();

		ItemData&										operator[](UInt32 a_pos);

		std::vector<ItemData>::iterator					begin() noexcept;
		std::vector<ItemData>::iterator					end() noexcept;

		bool											empty();
		UInt32											size();

		void											clear();
		std::vector<ItemData>::iterator					erase(std::vector<ItemData>::iterator a_pos);

	private:
		void											add(RE::InventoryEntryData* a_entryData);
		void											add(RE::InventoryEntryData* a_entryData, SInt32 a_count);
		void											add(TESForm* a_form, SInt32 a_count);

		void											sort();

		bool											isValidItem(TESForm* a_item);

		void											quicksort(SInt32 a_lo, SInt32 a_hi);
		UInt64											partition(SInt32 a_lo, SInt32 a_hi);
		ItemData&										pivot(SInt32 a_lo, SInt32 a_hi);

		typedef SInt32 Count;
		typedef UInt32 FormID;
		std::map<FormID, std::pair<TESForm*, Count>>	_defaultMap;
		std::vector<ItemData>							_itemList;
		std::vector<RE::InventoryEntryData*>			_heapList;
		std::vector<ItemData>::iterator					_toDelete;
		SInt32											_deleteCount;


		class TESContainerVisitor
		{
		public:
			explicit TESContainerVisitor(std::map<FormID, std::pair<TESForm*, Count>>& a_defaultMap);

			virtual bool Accept(TESContainer::Entry* a_entry);

		private:
			std::map<FormID, std::pair<TESForm*, Count>>& _defaultMap;
		};


		class EntryDataListVisitor
		{
		public:
			explicit EntryDataListVisitor(std::map<FormID, std::pair<TESForm*, Count>>& a_defaultMap);

			virtual bool Accept(InventoryEntryData* a_entryData);

		private:
			std::map<FormID, std::pair<TESForm*, Count>>& _defaultMap;
		};
	};


	extern InventoryList g_invList;
}
