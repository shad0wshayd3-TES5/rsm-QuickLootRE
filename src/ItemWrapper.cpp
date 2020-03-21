#include "ItemWrapper.h"

#include <cassert>


ItemWrapper::ItemWrapper(RE::InventoryEntryData* a_entryData) :
	_impl(std::move(a_entryData))
{
	assert(a_entryData);
}


ItemWrapper::ItemWrapper(const RE::NiPointer<RE::TESObjectREFR>& a_objectRef) :
	_impl(a_objectRef)
{
	assert(a_objectRef);
}


ItemWrapper::ItemWrapper(RE::NiPointer<RE::TESObjectREFR>&& a_objectRef) :
	_impl(std::move(a_objectRef))
{
	assert(a_objectRef);
}


RE::FormType ItemWrapper::GetFormType() const
{
	auto obj = GetObject();
	return obj ? obj->GetFormType() : RE::FormType::None;
}


const char* ItemWrapper::GetName() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->GenerateName();
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			return world->GetName();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


RE::TESBoundObject* ItemWrapper::GetObject() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->GetObject();
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			return world->GetBaseObject();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


RE::TESForm* ItemWrapper::GetOwner() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->GetOwner();
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			return world->GetOwner();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


RE::SOUL_LEVEL ItemWrapper::GetSoulLevel() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->GetSoulLevel();
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			auto xSoul = world->extraList.GetByType<RE::ExtraSoul>();
			if (xSoul && xSoul->soul > RE::SOUL_LEVEL::kNone) {
				return xSoul->soul;
			} else {
				auto obj = world->GetBaseObject();
				auto gem = obj ? obj->As<RE::TESSoulGem>() : 0;
				return gem ? gem->GetContainedSoul() : RE::SOUL_LEVEL::kNone;
			}
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


SInt32 ItemWrapper::GetValue() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->GetValue();
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			return world->GetGoldValue();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


float ItemWrapper::GetWeight() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->GetValue();
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			return world->GetGoldValue();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


bool ItemWrapper::HasExtraEnchantment() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			if (!inv->extraLists) {
				return false;
			}

			for (auto& xList : *inv->extraLists) {
				if (xList->HasType<RE::ExtraEnchantment>()) {
					return true;
				}
			}

			return false;
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			return world->extraList.HasType<RE::ExtraEnchantment>();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


bool ItemWrapper::Is(RE::FormType a_type) const
{
	auto obj = GetObject();
	return obj && obj->Is(a_type);
}


bool ItemWrapper::IsNot(RE::FormType a_type) const
{
	return !Is(a_type);
}


bool ItemWrapper::IsOwnedBy(RE::Actor* a_actor, RE::TESForm* a_itemOwner) const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			return inv->IsOwnedBy(a_actor, a_itemOwner);
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			InventoryEntryDataWrapper wrapper(world);	// need a better solution for this
			return wrapper->IsOwnedBy(a_actor, a_itemOwner);
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


bool ItemWrapper::IsWorn() const
{
	switch (_impl.index()) {
	case kInventory:
		{
			auto inv = std::get<kInventory>(_impl);
			if (!inv->extraLists) {
				return false;
			}

			for (auto& xList : *inv->extraLists) {
				if (!xList) {
					continue;
				}

				if (xList->HasType<RE::ExtraWorn>() || xList->HasType<RE::ExtraWornLeft>()) {
					return true;
				}
			}

			return false;
		}
	case kWorld:
		{
			auto world = std::get<kWorld>(_impl);
			auto& xList = world->extraList;
			return xList.HasType<RE::ExtraWorn>() || xList.HasType<RE::ExtraWornLeft>();
		}
	default:
		throw Exception_t(ERR_MSG);
	}
}


ItemWrapper::InventoryEntryDataWrapper::InventoryEntryDataWrapper(RE::TESObjectREFR* a_ref) :
	_data(a_ref->GetBaseObject(), 0)
{}


ItemWrapper::InventoryEntryDataWrapper::InventoryEntryDataWrapper(const RE::NiPointer<RE::TESObjectREFR>& a_ref) :
	_data(a_ref->GetBaseObject(), 0)
{
	_data.AddExtraList(&a_ref->extraList);
}


RE::InventoryEntryData* ItemWrapper::InventoryEntryDataWrapper::operator->()
{
	return get();
}


RE::InventoryEntryData* ItemWrapper::InventoryEntryDataWrapper::get()
{
	return &_data;
}
