#pragma once

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

		inline InventoryItem(std::ptrdiff_t a_count, std::unique_ptr<RE::InventoryEntryData> a_item, RE::ObjectRefHandle a_container) :
			super(a_count, a_item.get()),
			_entry(std::move(a_item)),
			_container(a_container)
		{
			assert(_entry != nullptr);
			assert(_entry->GetObject() != nullptr);
			assert(_container);
		}

		~InventoryItem() = default;

		InventoryItem& operator=(const InventoryItem&) = delete;
		InventoryItem& operator=(InventoryItem&&) = default;

	protected:
		inline void DoTake(observer<RE::Actor*> a_dst, std::ptrdiff_t a_count) override
		{
			assert(a_dst != nullptr);
			auto container = _container.get();
			if (!container) {
				assert(false);
				return;
			}

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
			for (const auto& [xList, count] : queued) {
				container->RemoveItem(object, static_cast<SInt32>(count), RE::ITEM_REMOVE_REASON::kRemove, xList, a_dst);
			}
			if (toRemove > 0) {
				container->RemoveItem(object, static_cast<SInt32>(toRemove), RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_dst);
				toRemove = 0;
			}
		}

	private:
		std::unique_ptr<RE::InventoryEntryData> _entry;
		RE::ObjectRefHandle _container;
	};
}
