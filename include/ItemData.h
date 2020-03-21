#pragma once

#include "RE/Skyrim.h"

#include "GFxItemData.h"


class ItemData
{
public:
	ItemData() = delete;
	ItemData(const ItemData&) = default;
	ItemData(ItemData&&) = default;
	ItemData(RE::InventoryEntryData* a_entryData, SInt32 a_count);
	ItemData(const RE::NiPointer<RE::TESObjectREFR>& a_objectRef, SInt32 a_count);
	ItemData(RE::NiPointer<RE::TESObjectREFR>&& a_objectRef, SInt32 a_count);
	~ItemData() = default;

	ItemData& operator=(const ItemData&) = default;
	ItemData& operator=(ItemData&&) = default;

	friend bool operator==(const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool operator!=(const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool operator< (const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool operator> (const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool operator<=(const ItemData& a_lhs, const ItemData& a_rhs);
	friend bool operator>=(const ItemData& a_lhs, const ItemData& a_rhs);

	static void SetContainer(RE::TESObjectREFR* a_container);

	const char* GetName() const;
	SInt32 GetCount() const;
	SInt32 GetValue() const;
	float GetWeight() const;
	const char* GetIcon() const;
	bool GetIsStolen() const;
	bool GetIsRead() const;
	bool GetIsEnchanted() const;
	bool GetCanPickPocket() const;
	SInt32 GetPickPocketChance() const;

	void Take(RE::Actor* a_to, RE::TESObjectREFR* a_from, SInt32 a_count = 1);
	void TakeAll(RE::Actor* a_to, RE::TESObjectREFR* a_from);

private:
	void AddItem(RE::Actor* a_to, RE::TESObjectREFR* a_from, SInt32 a_count);
	void PickupObject(RE::Actor* a_to, RE::TESObjectREFR* a_from, SInt32 a_count);


	GFxItemData _gfxItem;
	RE::BSSimpleList<RE::ExtraDataList*>* _extraLists;
	RE::NiPointer<RE::TESObjectREFR> _worldRef;
	RE::TESBoundObject* _object;
	SInt32 _count;
};
