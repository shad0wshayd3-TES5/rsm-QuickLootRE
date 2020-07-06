#pragma once

#include "Items/GFxItem.h"

namespace Items
{
	class Item
	{
	public:
		Item() = delete;
		Item(const Item&) = delete;
		Item(Item&&) = default;

		inline Item(observer<RE::InventoryEntryData*> a_item, std::ptrdiff_t a_count) :
			_item(a_count, a_item)
		{}

		inline Item(const std::vector<RE::ObjectRefHandle>& a_items, std::ptrdiff_t a_count) :
			_item(a_count, a_items)
		{}

		virtual ~Item() = default;

		Item& operator=(const Item&) = delete;
		Item& operator=(Item&&) = default;

		[[nodiscard]] inline int Compare(const Item& a_rhs) const { return _item.Compare(a_rhs._item); }

		[[nodiscard]] inline std::ptrdiff_t Count() const { return std::max<std::ptrdiff_t>(_item.Count(), 0); }
		[[nodiscard]] inline RE::GFxValue Value() const { return _item.Value(); }

		inline void Take(observer<RE::Actor*> a_dst, std::ptrdiff_t a_count) { DoTake(a_dst, a_count); }
		inline void Take(observer<RE::Actor*> a_dst) { DoTake(a_dst, 1); }
		inline void TakeAll(observer<RE::Actor*> a_dst) { DoTake(a_dst, Count()); }

	protected:
		virtual void DoTake(observer<RE::Actor*> a_dst, std::ptrdiff_t a_count) = 0;

	private:
		GFxItem _item;
	};

	[[nodiscard]] inline bool operator==(const Item& a_lhs, const Item& a_rhs) { return a_lhs.Compare(a_rhs) == 0; }
	[[nodiscard]] inline bool operator!=(const Item& a_lhs, const Item& a_rhs) { return a_lhs.Compare(a_rhs) != 0; }
	[[nodiscard]] inline bool operator<(const Item& a_lhs, const Item& a_rhs) { return a_lhs.Compare(a_rhs) < 0; }
	[[nodiscard]] inline bool operator>(const Item& a_lhs, const Item& a_rhs) { return a_lhs.Compare(a_rhs) > 0; }
	[[nodiscard]] inline bool operator<=(const Item& a_lhs, const Item& a_rhs) { return a_lhs.Compare(a_rhs) <= 0; }
	[[nodiscard]] inline bool operator>=(const Item& a_lhs, const Item& a_rhs) { return a_lhs.Compare(a_rhs) >= 0; }
}
