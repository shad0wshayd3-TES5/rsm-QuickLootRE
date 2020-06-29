#pragma once

#include "CLIK/Object.h"
#include "Items/Item.h"

namespace Items
{
	class InventoryItem :
		public Item
	{
	private:
		using super = Item;

	public:
		InventoryItem() = delete;
		InventoryItem(const InventoryItem&) = delete;
		InventoryItem(InventoryItem&&) = default;

		inline InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_item, std::ptrdiff_t a_count) :
			InventoryItem(std::move(a_item), a_count, RE::ObjectRefHandle())
		{}

		inline InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_item, std::ptrdiff_t a_count, RE::TESObjectREFRPtr a_container) :
			InventoryItem(std::move(a_item), a_count, a_container->CreateRefHandle())
		{}

		inline InventoryItem(std::unique_ptr<RE::InventoryEntryData> a_item, std::ptrdiff_t a_count, RE::ObjectRefHandle a_container) :
			super(a_item.get(), a_count),
			_entry(std::move(a_item)),
			_container(a_container)
		{}

		~InventoryItem() = default;

		InventoryItem& operator=(const InventoryItem&) = delete;
		InventoryItem& operator=(InventoryItem&&) = default;

	protected:
		inline void DoTake(RE::ActorPtr a_dst, std::ptrdiff_t a_count) override
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
						const auto xCount = std::min<std::ptrdiff_t>(xList->GetCount(), toRemove);
						toRemove -= xCount;
						queued.emplace_back(xList, xCount);

						if (toRemove <= 0) {
							break;
						}
					}
				}
			}

			const auto object = _entry->GetObject();
			const auto container = _container.get();
			for (const auto& [xList, count] : queued) {
				a_dst->AddObjectToContainer(object, xList, static_cast<SInt32>(count), container.get());
				_count -= count;
			}
			if (toRemove > 0) {
				a_dst->AddObjectToContainer(object, nullptr, static_cast<SInt32>(toRemove), container.get());
				_count -= toRemove;
				toRemove = 0;
			}
		}

	private:
		std::unique_ptr<RE::InventoryEntryData> _entry;
		RE::ObjectRefHandle _container;
	};
}
