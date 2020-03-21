#pragma once

#include <stdexcept>
#include <variant>

#include "RE/Skyrim.h"


class ItemWrapper
{
public:
	explicit ItemWrapper(RE::InventoryEntryData* a_entryData);
	explicit ItemWrapper(const RE::NiPointer<RE::TESObjectREFR>& a_objectRef);
	explicit ItemWrapper(RE::NiPointer<RE::TESObjectREFR>&& a_objectRef);
	~ItemWrapper() = default;

	RE::FormType GetFormType() const;
	const char* GetName() const;
	RE::TESBoundObject* GetObject() const;
	template <class T> T* GetObject() const;
	template <class T> T* GetObjectAs() const;
	RE::TESForm* GetOwner() const;
	RE::SOUL_LEVEL GetSoulLevel() const;
	SInt32 GetValue() const;
	float GetWeight() const;
	bool HasExtraEnchantment() const;
	bool Is(RE::FormType a_type) const;
	bool IsNot(RE::FormType a_type) const;
	bool IsOwnedBy(RE::Actor* a_actor, RE::TESForm* a_itemOwner) const;
	bool IsWorn() const;

private:
	class InventoryEntryDataWrapper
	{
	public:
		InventoryEntryDataWrapper(RE::TESObjectREFR* a_ref);
		InventoryEntryDataWrapper(const RE::NiPointer<RE::TESObjectREFR>& a_ref);
		~InventoryEntryDataWrapper() = default;

		RE::InventoryEntryData* operator->();
		RE::InventoryEntryData* get();

	private:
		RE::InventoryEntryData _data;
	};


	using Exception_t = std::out_of_range;


	enum
	{
		kInventory = 0,
		kWorld
	};


	static constexpr char ERR_MSG[] = "Variant had more values than expected!";
	std::variant<RE::InventoryEntryData*, RE::NiPointer<RE::TESObjectREFR>> _impl;
};


template <class T>
T* ItemWrapper::GetObject() const
{
	return static_cast<T*>(GetObject());
}


template <class T>
T* ItemWrapper::GetObjectAs() const
{
	auto obj = GetObject();
	return obj ? obj->As<T>() : 0;
}
