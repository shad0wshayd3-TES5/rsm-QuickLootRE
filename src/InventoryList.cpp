#include "InventoryList.h"

#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include <algorithm>  // sort
#include <utility>  // pair
#include <map>  // map
#include <vector>  // vector

#include "Forms.h"

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/BSFixedString.h"  // BSFixedString
#include "RE/BSTArray.h"  // BSScrapArray
#include "RE/ExtraContainerChanges.h"  // ExtraContainerChanges
#include "RE/ExtraDataTypes.h"  // ExtraDataType
#include "RE/ExtraDroppedItemList.h"  // ExtraDroppedItemList
#include "RE/InventoryChanges.h"  // InventoryChanges
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/PlayerCharacter.h"  // PlayerCharacter
#include "RE/TESContainer.h"  // TESContainer::Entry
#include "RE/TESForm.h"  // TESForm
#include "RE/TESFullName.h"  // TESFullName
#include "RE/TESLeveledList.h"  // TESLeveledList
#include "RE/TESLevItem.h"  // TESLevItem
#include "RE/TESObjectREFR.h"  // TESObjectREFR
#include "RE/TESObjectLIGH.h"  // TESObjectLIGH


namespace QuickLootRE
{
	InventoryList::InventoryList()
	{}


	InventoryList::~InventoryList()
	{
		clear();
	}


	void InventoryList::parseInventory(RE::TESObjectREFR* a_refr)
	{
		clear();

		if (!a_refr) {
			ItemData::setContainer(0);
			return;
		}

		ItemData::setContainer(a_refr);

		// Get extra items
		parseInventoryChanges(a_refr);
		parseDroppedList(a_refr);

		// Get default items
		TESContainerVisitor containerOp(_defaultMap, _heapList);
		a_refr->GetContainer()->Visit(containerOp);

		// Add parsed items
		for (auto& it : _defaultMap) {
			add(it.second.first, it.second.second);
		}

		std::sort(_itemList.begin(), _itemList.end(), operator>);
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
			delete entryData;
		}
		_heapList.clear();
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


	void InventoryList::parseInventoryChanges(RE::TESObjectREFR* a_refr)
	{
		RE::InventoryChanges* invChanges = a_refr->GetInventoryChanges();

		if (!invChanges || !invChanges->entryList) {
			return;
		}

		for (auto& entry : *invChanges->entryList) {
			_defaultMap.emplace(entry->type->formID, std::make_pair(entry, entry->countDelta));
		}
	}


	void InventoryList::parseDroppedList(RE::TESObjectREFR* a_refr)
	{
		RE::ExtraDroppedItemList* droppedList = static_cast<RE::ExtraDroppedItemList*>(a_refr->extraData.GetByType(RE::ExtraDataType::kDroppedItemList));
		if (!droppedList) {
			return;
		}

		for (auto& handle : droppedList->handles) {
			if (handle == *g_invalidRefHandle) {
				continue;
			}

			RE::TESObjectREFRPtr refPtr;
			if (!RE::TESObjectREFR::LookupByHandle(handle, refPtr)) {
				continue;
			}

			RE::InventoryEntryData* entryData = new RE::InventoryEntryData(refPtr->baseForm, 1);
			entryData->AddEntryList(&refPtr->extraData);
			_heapList.push_back(entryData);
			_defaultMap.emplace(entryData->type->formID, std::make_pair(entryData, entryData->countDelta));
		}
	}


	bool InventoryList::isValidItem(RE::TESForm* a_item)
	{
		using RE::TESFullName;

		using RE::FormType;

		if (!a_item) {
			return false;
		}

		switch (a_item->formType) {
		case FormType::ScrollItem:
		case FormType::Armor:
		case FormType::Book:
		case FormType::Ingredient:
		case FormType::Misc:
		case FormType::Weapon:
		case FormType::Ammo:
		case FormType::Key:
		case FormType::Potion:
		case FormType::Note:
		case FormType::SoulGem:
			break;
		case FormType::Light:
		{
			RE::TESObjectLIGH* light = static_cast<RE::TESObjectLIGH*>(a_item);
			if (!light->CanBeCarried()) {
				return false;
			} else {
				break;
			}
		}
		default:
			return false;
		}

		if (!a_item->IsPlayable()) {
			return false;
		}

		static RE::BSFixedString emptyStr = "";
		RE::TESFullName* fullName = 0;
		fullName = DYNAMIC_CAST(a_item, TESForm, TESFullName);
		if (!fullName || fullName->name == emptyStr) {
			return false;
		}

		return true;
	}


	InventoryList::TESContainerVisitor::TESContainerVisitor(std::map<FormID, std::pair<RE::InventoryEntryData*, Count>>& a_defaultMap, std::vector<RE::InventoryEntryData*>& a_heapList) :
		_defaultMap(a_defaultMap),
		_heapList(a_heapList)
	{}


	bool InventoryList::TESContainerVisitor::Accept(RE::TESContainer::Entry* a_entry)
	{
		auto& it = _defaultMap.find(a_entry->form->formID);
		if (it != _defaultMap.end()) {
			if (a_entry->form->formID != kMISCFormID_Gold) {
				it->second.second += a_entry->count;
			}
		} else {
			RE::InventoryEntryData* entryData = new RE::InventoryEntryData(a_entry->form, a_entry->count);
			_heapList.push_back(entryData);
			_defaultMap.emplace(entryData->type->formID, std::make_pair(entryData, entryData->countDelta));
		}
		return true;
	}


	InventoryList g_invList;
}
