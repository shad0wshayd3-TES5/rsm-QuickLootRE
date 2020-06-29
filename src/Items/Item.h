#pragma once

#include "GFxItem.h"

namespace Items
{
	class Item
	{
	public:
		Item() = delete;
		Item(const Item&) = delete;
		Item(Item&&) = default;

		inline Item(observer<RE::InventoryEntryData*> a_item, std::ptrdiff_t a_count) :
			_count(a_count),
			_item(a_item)
		{}

		inline Item(observer<RE::TESObjectREFR*> a_item, std::ptrdiff_t a_count) :
			_count(a_count),
			_item(a_item)
		{}

		virtual ~Item() = 0 {}

		Item& operator=(const Item&) = delete;
		Item& operator=(Item&&) = default;

		[[nodiscard]] inline std::ptrdiff_t Count() const
		{
			return std::max<std::ptrdiff_t>(_count, 0);
		}

		[[nodiscard]] inline RE::GFxValue Value() const
		{
			return _item.Value();
		}

		inline void Take(observer<RE::Actor*> a_dst, std::ptrdiff_t a_count)
		{
			DoTake(a_dst, a_count);
		}

		inline void Take(observer<RE::Actor*> a_dst)
		{
			DoTake(a_dst, 1);
		}

		inline void TakeAll(observer<RE::Actor*> a_dst)
		{
			DoTake(a_dst, _count);
		}

	protected:
		virtual void DoTake(observer<RE::Actor*> a_dst, std::ptrdiff_t a_count) = 0;

		std::ptrdiff_t _count;

	private:
		GFxItem _item;
	};
}
