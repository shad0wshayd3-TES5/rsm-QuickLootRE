#include "InventoryList.h"

#include "skse64/GameReferences.h"

#include <algorithm>

#include "RE/Skyrim.h"


void InventoryList::parse(RE::TESObjectREFR* a_ref)
{
	_itemList.clear();

	if (!a_ref) {
		ItemData::SetContainer(0);
		return;
	} else {
		ItemData::SetContainer(a_ref);
	}

	auto inv = a_ref->GetInventory(is_valid);
	for (auto& item : inv) {
		auto count = item.second.first;
		if (count > 0) {
			auto entry = ItemData(item.second.second, item.second.first);
			_itemList.emplace_back(std::move(entry), item.second.first);
		}
	}

	auto droppedInv = a_ref->GetDroppedInventory(is_valid);
	for (auto& item : droppedInv) {
		auto count = item.second.first;
		if (count > 0) {
			auto entry = ItemData(std::move(item.second.second), item.second.first);
			_itemList.emplace_back(std::move(entry), item.second.first);
		}
	}

	std::sort(_itemList.begin(), _itemList.end(), std::greater<ItemData>());
}


auto InventoryList::operator[](size_type a_pos)
-> reference
{
	return _itemList[a_pos];
}


auto InventoryList::operator[](size_type a_pos) const
-> const_reference
{
	return _itemList[a_pos];
}


auto InventoryList::begin() noexcept
-> iterator
{
	return _itemList.begin();
}


auto InventoryList::begin() const noexcept
-> const_iterator
{
	return _itemList.begin();
}


auto InventoryList::cbegin() const noexcept
-> const_iterator
{
	return _itemList.cbegin();
}


auto InventoryList::end() noexcept
-> iterator
{
	return _itemList.end();
}


auto InventoryList::end() const noexcept
-> const_iterator
{
	return _itemList.end();
}


auto InventoryList::cend() const noexcept
-> const_iterator
{
	return _itemList.cend();
}


bool InventoryList::empty() const noexcept
{
	return _itemList.empty();
}


auto InventoryList::size() const noexcept
-> size_type
{
	return _itemList.size();
}


void InventoryList::clear() noexcept
{
	ItemData::SetContainer(0);
	_itemList.clear();
}


auto InventoryList::erase(const_iterator a_pos)
-> iterator
{
	return _itemList.erase(a_pos);
}


auto InventoryList::erase(const_iterator a_first, const_iterator a_last)
-> iterator
{
	return _itemList.erase(a_first, a_last);
}


bool InventoryList::is_valid(RE::TESBoundObject* a_item)
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

	if (!a_item->GetPlayable()) {
		return false;
	}

	auto name = a_item->GetName();
	if (name[0] == '\0') {
		return false;
	}

	return true;
}
