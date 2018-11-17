#include "InventoryList.h"

#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESFullName
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/GameTypes.h"  // tArray

#include <algorithm>  // sort
#include <utility>  // pair
#include <exception>  // exception
#include <map>  // map
#include <vector>  // vector

#include "LootMenu.h"

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/BSTArray.h"  // BSScrapArray
#include "RE/InventoryChanges.h"  // InventoryChanges
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESLeveledList.h"  // TESLeveledList
#include "RE/TESLevItem.h"  // TESLevItem
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/TESObjectLIGH.h"  // TESObjectLIGH


namespace QuickLootRE
{
	InventoryList::InventoryList() :
		_toDelete(end()),
		_deleteCount(0)
	{}


	InventoryList::~InventoryList()
	{
		clear();
	}


	void InventoryList::parseInventory(RE::TESObjectREFR* a_refr)
	{
		clear();
		ItemData::setContainer(a_refr);

		// Get default items
		TESContainerVisitor containerOp(_defaultMap);
		a_refr->GetContainer()->Visit(containerOp);

		// Parse changes
		bool changesCreated = !a_refr->HasInventoryChanges();
		RE::InventoryChanges* changes = a_refr->GetInventoryChanges();
		if (changesCreated) {
			changes->InitContainer();
			changes->GenerateLeveledListChanges();
		}
		if (changes && changes->entryList) {
			for (auto& entry : *changes->entryList) {
				auto it = _defaultMap.find(entry->type->formID);
				if (it != _defaultMap.end()) {
					_defaultMap.erase(entry->type->formID);
				}
				add(entry);
			}
		}

		// Add remaining default items
		for (auto& it : _defaultMap) {
			add(it.second.first, it.second.second);
		}

		std::sort(_itemList.rbegin(), _itemList.rend());
	}


	ItemData& InventoryList::operator[](UInt32 a_pos)
	{
		return _itemList[a_pos];
	}


	std::vector<ItemData>::iterator InventoryList::begin() noexcept
	{
		return _itemList.begin();
	}


	std::vector<ItemData>::iterator InventoryList::end() noexcept
	{
		return _itemList.end();
	}


	bool InventoryList::empty()
	{
		return _itemList.empty();
	}


	UInt32 InventoryList::size()
	{
		return _itemList.size();
	}


	void InventoryList::clear()
	{
		ItemData::setContainer(0);
		_defaultMap.clear();
		_itemList.clear();
		for (auto& entryData : _heapList) {
			entryData->Delete();
		}
		_heapList.clear();
		_toDelete = end();
		_deleteCount = 0;
	}


	std::vector<ItemData>::iterator InventoryList::erase(std::vector<ItemData>::iterator a_pos)
	{
		return _itemList.erase(a_pos);
	}


	void InventoryList::add(RE::InventoryEntryData* a_entryData)
	{
		if (isValidItem(a_entryData->type) && a_entryData->countDelta > 0) {
			_itemList.emplace_back(a_entryData);
		}
	}


	void InventoryList::add(RE::InventoryEntryData* a_entryData, SInt32 a_count)
	{
		if (isValidItem(a_entryData->type) && a_count > 0) {
			_itemList.emplace_back(a_entryData, a_count);
		}
	}


	void InventoryList::add(TESForm* a_form, SInt32 a_count)
	{
		RE::InventoryEntryData* entryData = RE::InventoryEntryData::Create(a_form, a_count);
		_heapList.push_back(entryData);
		add(entryData);
	}


	bool InventoryList::isValidItem(TESForm* a_item)
	{
		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		if (!a_item) {
			return false;
		}

		if (a_item->formType == kFormType_LeveledItem) {
			return false;
		}

		if (a_item->formType == kFormType_Light) {
			RE::TESObjectLIGH* light = static_cast<RE::TESObjectLIGH*>(a_item);
			if (!light->CanBeCarried()) {
				return false;
			}
		} else if (!a_item->IsPlayable()) {
			return false;
		}

		TESFullName* fullName = 0;
		try {
			static BSFixedString emptyStr = "";
			fullName = DYNAMIC_CAST(a_item, TESForm, TESFullName);
			if (!fullName || fullName->name == emptyStr) {
				return false;
			}
		} catch (std::exception& e) {
			_ERROR("[ERROR] Form (0x%X) does not have TESFullName (%i)\n", a_item->formID, a_item->formType);
			_ERROR(e.what());
			return false;
		}

		return true;
	}


	InventoryList::TESContainerVisitor::TESContainerVisitor(std::map<FormID, std::pair<TESForm*, Count>>& a_defaultMap) :
		_defaultMap(a_defaultMap)
	{}


	bool InventoryList::TESContainerVisitor::Accept(TESContainer::Entry* a_entry)
	{
		_defaultMap.emplace(a_entry->form->formID, std::pair<TESForm*, Count>(a_entry->form, a_entry->count));
		return true;
	}


	InventoryList g_invList;
}
