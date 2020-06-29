#pragma once

#include "Items/Item.h"

namespace Items
{
	class GroundItems :
		public Item
	{
	private:
		using super = Item;

	public:
		GroundItems() = delete;
		GroundItems(const GroundItems&) = delete;
		GroundItems(GroundItems&&) = default;

		inline GroundItems(std::vector<RE::NiPointer<RE::TESObjectREFR>> a_items, std::ptrdiff_t a_count) :
			super(a_items.front().get(), a_count),
			_items(std::move(a_items))
		{}

		~GroundItems() = default;

		GroundItems& operator=(const GroundItems&) = delete;
		GroundItems& operator=(GroundItems&&) = default;

	protected:
		inline void DoTake(RE::ActorPtr a_dst, std::ptrdiff_t a_count) override
		{
			auto toRemove = std::clamp<std::ptrdiff_t>(a_count, 0, Count());
			if (toRemove <= 0) {
				assert(false);
				return;
			}

			for (const auto& item : _items) {
				const auto xCount = std::min<std::ptrdiff_t>(item->extraList.GetCount(), toRemove);
				a_dst->PickUpObject(item.get(), static_cast<SInt32>(xCount));
				_count -= xCount;
				toRemove -= xCount;

				if (toRemove <= 0) {
					break;
				}
			}
		}

	private:
		std::vector<RE::NiPointer<RE::TESObjectREFR>> _items;
	};
}
