#include "InventoryList.h"

#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include <algorithm>  // sort
#include <map>  // map
#include <vector>  // vector

#include "Forms.h"
#include "ManagedEntryData.h"  // ManagedEntryDataPtr

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



InventoryList::EntryDataCountPair::EntryDataCountPair(ManagedEntryDataPtr a_entryData, SInt32 a_count) :
	entryData{ a_entryData },
	count{ a_count }
{}


InventoryList& InventoryList::GetSingleton()
{
	return _singleton;
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
	TESContainerVisitor containerOp(_defaultMap);
	a_refr->GetContainer()->Visit(containerOp);

	// Add parsed items
	for (auto& it : _defaultMap) {
		add(it.second.entryData, it.second.count);
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


bool InventoryList::empty() const noexcept
{
	return _itemList.empty();
}


UInt32 InventoryList::size() const noexcept
{
	return _itemList.size();
}


void InventoryList::clear() noexcept
{
	ItemData::setContainer(0);
	_defaultMap.clear();
	_itemList.clear();
}


std::vector<ItemData>::iterator InventoryList::erase(std::vector<ItemData>::iterator a_pos)
{
	return _itemList.erase(a_pos);
}


InventoryList::InventoryList()
{}


InventoryList::~InventoryList()
{
	clear();
}


void InventoryList::add(ManagedEntryDataPtr& a_entryData)
{
	if (isValidItem(a_entryData->Get()->type) && a_entryData->Get()->countDelta > 0) {
		_itemList.emplace_back(a_entryData);
	}
}


void InventoryList::add(ManagedEntryDataPtr& a_entryData, SInt32 a_count)
{
	if (isValidItem(a_entryData->Get()->type) && a_count > 0) {
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
		_defaultMap.insert({ entry->type->formID, EntryDataCountPair(MakeEntryDataPtr(entry, false), entry->countDelta) });
	}
}


void InventoryList::parseDroppedList(RE::TESObjectREFR* a_refr)
{
	RE::ExtraDroppedItemList* droppedList = a_refr->extraData.GetByType<RE::ExtraDroppedItemList>();
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

		ManagedEntryDataPtr entryData = MakeEntryDataPtr(refPtr->baseForm, 1);
		entryData->Get()->AddEntryList(&refPtr->extraData);
		auto result = _defaultMap.insert({ entryData->Get()->type->formID, EntryDataCountPair(entryData, entryData->Get()->countDelta) });
		if (!result.second) {
			auto& item = result.first->second;
			item.count += 1;
			if (item.entryData->IsManaged()) {
				item.entryData->Get()->AddEntryList(&refPtr->extraData);
			} else {
				if (item.entryData->Get()->extraList) {
					for (auto& xList : *item.entryData->Get()->extraList) {
						entryData->Get()->AddEntryList(xList);
					}
				}
				item.entryData = entryData;
			}
		}
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
	case FormType::Scroll:
	case FormType::Armor:
	case FormType::Book:
	case FormType::Ingredient:
	case FormType::Misc:
	case FormType::Weapon:
	case FormType::Ammo:
	case FormType::KeyMaster:
	case FormType::AlchemyItem:
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


InventoryList::TESContainerVisitor::TESContainerVisitor(DefaultMap& a_defaultMap) :
	_defaultMap(a_defaultMap)
{}


bool InventoryList::TESContainerVisitor::Accept(RE::TESContainer::Entry* a_entry)
{
	auto& it = _defaultMap.find(a_entry->form->formID);
	if (it != _defaultMap.end()) {
		if (a_entry->form->formID != kMISCFormID_Gold) {
			it->second.count += a_entry->count;
		}
	} else {
		ManagedEntryDataPtr entryData = MakeEntryDataPtr(a_entry->form, a_entry->count);
		_defaultMap.insert({ entryData->Get()->type->formID, EntryDataCountPair(entryData, entryData->Get()->countDelta) });
	}
	return true;
}


InventoryList InventoryList::_singleton;
