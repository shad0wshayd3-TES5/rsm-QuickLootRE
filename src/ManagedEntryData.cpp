#include "ManagedEntryData.h"


ManagedEntryData::ManagedEntryData(ManagedEntryData&& a_rhs) :
	_entryData{ std::move(a_rhs._entryData) },
	_isManaged{ std::move(a_rhs._isManaged) }
{
	a_rhs._entryData = 0;
	a_rhs._isManaged = false;
}


ManagedEntryData::ManagedEntryData(RE::TESBoundObject* a_item, SInt32 a_count) :
	_entryData{ new RE::InventoryEntryData{ a_item, a_count } },
	_isManaged{ true }
{}


ManagedEntryData::ManagedEntryData(RE::InventoryEntryData* a_entryData, bool a_manage) :
	_entryData{ a_entryData },
	_isManaged{ a_manage }
{}


ManagedEntryData::~ManagedEntryData()
{
	if (_isManaged) {
		delete _entryData;
	}
}


ManagedEntryData& ManagedEntryData::operator=(ManagedEntryData&& a_rhs)
{
	_entryData = std::move(a_rhs._entryData);
	a_rhs._entryData = 0;

	_isManaged = std::move(a_rhs._isManaged);
	a_rhs._isManaged = false;

	return *this;
}


RE::InventoryEntryData* ManagedEntryData::Get()
{
	return _entryData;
}


bool ManagedEntryData::IsManaged() const
{
	return _isManaged;
}


ManagedEntryDataPtr MakeEntryDataPtr(RE::TESBoundObject* a_item, SInt32 a_count)
{
	return ManagedEntryDataPtr{ new ManagedEntryData{a_item, a_count} };
}


ManagedEntryDataPtr MakeEntryDataPtr(RE::InventoryEntryData* a_entryData, bool a_manage)
{
	return ManagedEntryDataPtr{ new ManagedEntryData{a_entryData, a_manage} };
}
