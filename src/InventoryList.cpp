#include "InventoryList.h"

#include <algorithm>  // sort
#include <map>
#include <vector>

#include "Forms.h"
#include "ManagedEntryData.h"

#include "RE/Skyrim.h"


InventoryList::EntryDataCountPair::EntryDataCountPair(ManagedEntryDataPtr a_entryData, SInt32 a_count) :
	entryData{ a_entryData },
	count{ a_count }
{}


void InventoryList::parseInventory(RE::TESObjectREFR* a_refr)
{
	clear();

	if (!a_refr) {
		ItemData::SetContainer(0);
		return;
	} else {
		ItemData::SetContainer(a_refr);
	}

	// Get extra items
	parseInventoryChanges(a_refr);
	parseDroppedList(a_refr);

	// Get default items
	a_refr->GetContainer()->ForEach([&](auto a_entry)
	{
		auto& it = _defaultMap.find(a_entry->form->formID);
		if (it != _defaultMap.end()) {
			if (a_entry->form->formID != kGold) {
				it->second.count += a_entry->count;
			}
		} else {
			auto entryData = MakeEntryDataPtr(a_entry->form, a_entry->count);
			_defaultMap.insert({ entryData->Get()->type->formID, EntryDataCountPair(entryData, entryData->Get()->countDelta) });
		}
		return true;
	});

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


std::size_t InventoryList::size() const noexcept
{
	return _itemList.size();
}


void InventoryList::clear() noexcept
{
	ItemData::SetContainer(0);
	_defaultMap.clear();
	_itemList.clear();
}


std::vector<ItemData>::iterator InventoryList::erase(std::vector<ItemData>::iterator a_pos)
{
	return _itemList.erase(a_pos);
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
	auto invChanges = a_refr->GetInventoryChanges();
	if (!invChanges || !invChanges->entryList) {
		return;
	}

	for (auto& entry : *invChanges->entryList) {
		_defaultMap.insert({ entry->type->formID, EntryDataCountPair(MakeEntryDataPtr(entry, false), entry->countDelta) });
	}
}


void InventoryList::parseDroppedList(RE::TESObjectREFR* a_refr)
{
	auto droppedList = a_refr->extraData.GetByType<RE::ExtraDroppedItemList>();
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

		auto entryData = MakeEntryDataPtr(refPtr->baseForm, 1);
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
			auto light = static_cast<RE::TESObjectLIGH*>(a_item);
			if (!light->CanBeCarried()) {
				return false;
			}
		}
		break;
	default:
		return false;
	}

	if (!a_item->IsPlayable()) {
		return false;
	}

	auto fullName = skyrim_cast<RE::TESFullName*>(a_item);
	if (!fullName || fullName->name.empty()) {
		return false;
	}

	return true;
}
