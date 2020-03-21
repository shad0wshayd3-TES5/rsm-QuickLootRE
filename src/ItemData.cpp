#include "ItemData.h"

#include <algorithm>
#include <cassert>
#include <vector>

#include "ItemWrapper.h"


ItemData::ItemData(RE::InventoryEntryData* a_entryData, SInt32 a_count) :
	_gfxItem(std::move(ItemWrapper(a_entryData)), a_count),
	_extraLists(a_entryData->extraLists),
	_worldRef(),
	_object(a_entryData->object),
	_count(a_count)
{
	assert(_count > 0);
	assert(_object);
}


ItemData::ItemData(const RE::NiPointer<RE::TESObjectREFR>& a_objectRef, SInt32 a_count) :
	_gfxItem(std::move(ItemWrapper(a_objectRef)), a_count),
	_extraLists(0),
	_worldRef(a_objectRef),
	_object(a_objectRef->GetBaseObject()),
	_count(a_count)
{
	assert(_count > 0);
	assert(_object);
}


ItemData::ItemData(RE::NiPointer<RE::TESObjectREFR>&& a_objectRef, SInt32 a_count) :
	_gfxItem(std::move(ItemWrapper(a_objectRef)), a_count),
	_extraLists(0),
	_worldRef(std::move(a_objectRef)),
	_object(a_objectRef->GetBaseObject()),
	_count(a_count)
{
	assert(_count > 0);
	assert(_object);
}


bool operator==(const ItemData& a_lhs, const ItemData& a_rhs)
{
	return a_lhs._gfxItem == a_rhs._gfxItem;
}


bool operator!=(const ItemData& a_lhs, const ItemData& a_rhs)
{
	return a_lhs._gfxItem != a_rhs._gfxItem;
}


bool operator<(const ItemData& a_lhs, const ItemData& a_rhs)
{
	return a_lhs._gfxItem < a_rhs._gfxItem;
}


bool operator>(const ItemData& a_lhs, const ItemData& a_rhs)
{
	return a_lhs._gfxItem > a_rhs._gfxItem;
}


bool operator<=(const ItemData& a_lhs, const ItemData& a_rhs)
{
	return a_lhs._gfxItem <= a_rhs._gfxItem;
}


bool operator>=(const ItemData& a_lhs, const ItemData& a_rhs)
{
	return a_lhs._gfxItem >= a_rhs._gfxItem;
}


void ItemData::SetContainer(RE::TESObjectREFR* a_container)
{
	GFxItemData::SetContainer(a_container);
}


const char* ItemData::GetName() const
{
	return _gfxItem.GetName();
}


SInt32 ItemData::GetCount() const
{
	return _gfxItem.GetCount();
}


SInt32 ItemData::GetValue() const
{
	return _gfxItem.GetValue();
}


float ItemData::GetWeight() const
{
	return _gfxItem.GetWeight();
}


const char* ItemData::GetIcon() const
{
	return _gfxItem.GetIcon();
}


bool ItemData::GetIsStolen() const
{
	return _gfxItem.GetIsStolen();
}


bool ItemData::GetIsRead() const
{
	return _gfxItem.GetIsRead();
}


bool ItemData::GetIsEnchanted() const
{
	return _gfxItem.GetIsEnchanted();
}


bool ItemData::GetCanPickPocket() const
{
	return _gfxItem.GetCanPickPocket();
}


SInt32 ItemData::GetPickPocketChance() const
{
	return _gfxItem.GetPickPocketChance();
}


void ItemData::Take(RE::Actor* a_to, RE::TESObjectREFR* a_from, SInt32 a_count)
{
	assert(a_to);
	assert(a_count > 0);

	if (_count <= 0) {
		return;
	}

	_object->OnRemovedFrom(a_from);	// remove projectile 3D

	switch (a_from->GetFormType()) {
	case RE::FormType::ActorCharacter:
		{
			auto actor = static_cast<RE::Actor*>(a_from);
			if (!actor->IsDead(false)) {	// pickpocket
				return;
			}

			actor->DispelWornItemEnchantments();
			actor->Update3DModel();
		}
		break;
	default:
		break;
	}

	if (_worldRef) {
		PickupObject(a_to, a_from, a_count);
	} else {
		AddItem(a_to, a_from, a_count);
	}
}


void ItemData::TakeAll(RE::Actor* a_to, RE::TESObjectREFR* a_from)
{
	Take(a_to, a_from, _count);
}


void ItemData::AddItem(RE::Actor* a_to, RE::TESObjectREFR* a_from, SInt32 a_count)
{
	std::vector<std::pair<SInt32, RE::ExtraDataList*>> itemsToTake;
	auto targetCount = std::min(a_count, _count);

	if (_extraLists) {
		for (auto& xList : *_extraLists) {
			if (targetCount <= 0) {
				break;
			} else if (!xList) {
				continue;
			}

			auto count = xList->GetCount();
			if (count <= 0) {
				continue;
			}

			auto numToTake = std::min(count, targetCount);
			itemsToTake.push_back(std::make_pair(numToTake, xList));
			targetCount -= numToTake;
		}
	}

	if (targetCount > 0) {
		itemsToTake.push_back(std::make_pair(targetCount, nullptr));
		targetCount = 0;
	}

	for (auto& item : itemsToTake) {
		a_to->AddItem(_object, item.second, item.first, a_from);
	}
}


void ItemData::PickupObject(RE::Actor* a_to, RE::TESObjectREFR* a_from, SInt32 a_count)
{
	auto count = std::min(_count, a_count);
	a_to->PickUpObject(_worldRef.get(), count);
}
