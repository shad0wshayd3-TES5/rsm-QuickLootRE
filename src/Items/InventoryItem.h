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
		inline void DoTake(RE::Actor& a_dst, std::ptrdiff_t a_count) override
		{
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
			const auto remove =
				[&](SInt32 a_count, RE::ExtraDataList* a_extraList, RE::ITEM_REMOVE_REASON a_reason) {
					container->RemoveItem(object, a_count, a_reason, a_extraList, std::addressof(a_dst));
				};

			std::function action =
				[&](SInt32 a_num, RE::ExtraDataList* a_extraList) {
					remove(a_num, a_extraList, RE::ITEM_REMOVE_REASON::kRemove);
				};
			if (a_dst.WouldBeStealing(container.get())) {
				action =
					[&](SInt32 a_num, RE::ExtraDataList* a_extraList) {
						remove(a_num, a_extraList, RE::ITEM_REMOVE_REASON::kSteal);
						a_dst.StealAlarm(container.get(), object, a_num, Value(), container->GetOwner(), true);
					};
			}

			for (const auto& [xList, count] : queued) {
				action(static_cast<SInt32>(count), xList);
			}
			if (toRemove > 0) {
				action(static_cast<SInt32>(toRemove), nullptr);
			}
		}

	private:
		std::unique_ptr<RE::InventoryEntryData> _entry;
		RE::ObjectRefHandle _container;
	};
}
