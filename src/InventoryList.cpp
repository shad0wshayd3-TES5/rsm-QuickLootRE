#include "InventoryList.h"

#include "skse64/GameBSExtraData.h"  // BSExtraData
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

#include "Forms.h"

#include "RE/BaseExtraList.h"  // BaseExtraList
#include "RE/BSTArray.h"  // BSScrapArray
#include "RE/ExtraContainerChanges.h"  // ExtraContainerChanges
#include "RE/ExtraDroppedItemList.h"  // ExtraDroppedItemList
#include "RE/InventoryChanges.h"  // InventoryChanges
#include "RE/InventoryEntryData.h"  // InventoryEntryData
#include "RE/PlayerCharacter.h"  // PlayerCharacter
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

		// Get default items
		TESContainerVisitor containerOp(_defaultMap);
		a_refr->GetContainer()->Visit(containerOp);

		parseInventoryChanges(a_refr);
		parseDroppedList(a_refr);

		// Add remaining default items
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


	void InventoryList::add(TESForm* a_form, SInt32 a_count)
	{
		RE::InventoryEntryData* entryData = new RE::InventoryEntryData(a_form, a_count);
		_heapList.push_back(entryData);
		add(entryData);
	}


	void InventoryList::add(RE::TESObjectREFRPtr& a_refPtr, SInt32 a_count)
	{
		RE::InventoryEntryData* entryData = new RE::InventoryEntryData(a_refPtr->baseForm, a_count);
		entryData->AddEntryList(&a_refPtr->extraData);
		_heapList.push_back(entryData);
		add(entryData);
	}


	void InventoryList::parseInventoryChanges(RE::TESObjectREFR* a_refr)
	{
		bool createdChanges = !a_refr->HasInventoryChanges();
		RE::InventoryChanges* invChanges = a_refr->GetInventoryChanges();

		if (!invChanges) {
			return;
		}

		if (createdChanges) {
			invChanges->InitContainer();
			invChanges->GenerateLeveledListChanges();
		}

		if (!invChanges->entryList) {
			return;
		}

		for (auto& entry : *invChanges->entryList) {
			auto it = _defaultMap.find(entry->type->formID);
			if (it != _defaultMap.end()) {
				if (entry->type->formID == kMISCFormID_Gold) {
					_defaultMap.erase(entry->type->formID);  // Extra gold overrides the default container's gold. Not sure if this is intentional or a bug
					add(entry);
				} else {
					it->second.second += entry->countDelta;
				}
			} else {
				add(entry);
			}
		}
	}


	void InventoryList::parseDroppedList(RE::TESObjectREFR* a_refr)
	{
		RE::ExtraDroppedItemList* droppedList = static_cast<RE::ExtraDroppedItemList*>(a_refr->extraData.GetByType(kExtraData_DroppedItemList));
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

			add(refPtr, 1);
		}
	}


	bool InventoryList::isValidItem(TESForm* a_item)
	{
		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		if (!a_item) {
			return false;
		}

		switch (a_item->formType) {
		case kFormType_ScrollItem:
		case kFormType_Armor:
		case kFormType_Book:
		case kFormType_Ingredient:
		case kFormType_Misc:
		case kFormType_Weapon:
		case kFormType_Ammo:
		case kFormType_Key:
		case kFormType_Potion:
		case kFormType_Note:
		case kFormType_SoulGem:
			break;
		case kFormType_Light:
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

		static BSFixedString emptyStr = "";
		TESFullName* fullName = 0;
		fullName = DYNAMIC_CAST(a_item, TESForm, TESFullName);
		if (!fullName || fullName->name == emptyStr) {
			return false;
		}

		return true;
	}


	InventoryList::TESContainerVisitor::TESContainerVisitor(std::map<FormID, std::pair<TESForm*, Count>>& a_defaultMap) :
		_defaultMap(a_defaultMap)
	{}


	bool InventoryList::TESContainerVisitor::Accept(TESContainer::Entry* a_entry)
	{
		_defaultMap.emplace(a_entry->form->formID, std::make_pair(a_entry->form, a_entry->count));
		return true;
	}


	InventoryList g_invList;
}
