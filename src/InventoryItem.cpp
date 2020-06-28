#include "InventoryItem.h"

InventoryItem::InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_entry, std::ptrdiff_t a_count) :
	InventoryItem(std::move(a_entry), a_count, RE::ObjectRefHandle())
{}

InventoryItem::InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_entry, std::ptrdiff_t a_count, RE::ObjectRefHandle a_container) :
	_entry(std::move(a_entry)),
	_container(a_container),
	_count(a_count),
	_gfxItem(_entry)
{}

InventoryItem::InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_entry, std::ptrdiff_t a_count, RE::TESObjectREFRPtr a_container) :
	InventoryItem(std::move(a_entry), a_count, a_container->CreateRefHandle())
{}

[[nodiscard]] std::ptrdiff_t InventoryItem::Count() const
{
	return std::max<std::ptrdiff_t>(_count, 0);
}

CLIK::Object InventoryItem::Object() const
{
	return _gfxItem.Object();
}

void InventoryItem::Take(RE::TESObjectREFRPtr a_dst, std::ptrdiff_t a_count)
{
	auto toRemove = std::clamp<std::ptrdiff_t>(a_count, 0, Count());
	if (toRemove <= 0) {
		assert(false);
		return;
	}

	std::vector<std::pair<RE::ExtraDataList*, std::ptrdiff_t>> queued;
	if (_entry->extraLists) {
		for (auto& xList : *_entry->extraLists) {
			if (xList) {
				auto xCount = std::min<std::ptrdiff_t>(xList->GetCount(), toRemove);
				toRemove -= xCount;
				queued.push_back(std::make_pair(xList, xCount));

				if (toRemove <= 0) {
					break;
				}
			}
		}
	}

	auto object = _entry->GetObject();
	auto container = _container.get();
	for (auto& elem : queued) {
		a_dst->AddObjectToContainer(object, elem.first, static_cast<SInt32>(elem.second), container.get());
	}
	if (toRemove > 0) {
		a_dst->AddObjectToContainer(object, nullptr, static_cast<SInt32>(toRemove), container.get());
		toRemove = 0;
	}
}

void InventoryItem::Take(RE::TESObjectREFRPtr a_dst)
{
	Take(std::move(a_dst), 1);
}

void InventoryItem::TakeAll(RE::TESObjectREFRPtr a_dst)
{
	Take(std::move(a_dst), _count);
}
