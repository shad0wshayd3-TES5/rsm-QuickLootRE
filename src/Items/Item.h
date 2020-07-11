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

		inline Item(std::ptrdiff_t a_count, observer<RE::InventoryEntryData*> a_item) :
			_item(a_count, a_item)
		{}

		inline Item(std::ptrdiff_t a_count, stl::span<const RE::ObjectRefHandle> a_items) :
			_item(a_count, a_items)
		{}

		virtual ~Item() = default;

		Item& operator=(const Item&) = delete;
		Item& operator=(Item&&) = default;

		[[nodiscard]] inline int Compare(const Item& a_rhs) const { return _item.Compare(a_rhs._item); }

		[[nodiscard]] inline RE::GFxValue GFxValue(RE::GFxMovieView& a_view) const { return _item.GFxValue(a_view); }

		inline void Take(RE::Actor& a_dst, std::ptrdiff_t a_count) { DoTake(a_dst, a_count); }
		inline void Take(RE::Actor& a_dst) { DoTake(a_dst, 1); }
		inline void TakeAll(RE::Actor& a_dst) { DoTake(a_dst, Count()); }

	protected:
		virtual void DoTake(RE::Actor& a_dst, std::ptrdiff_t a_count) = 0;

		[[nodiscard]] inline std::ptrdiff_t Count() const { return std::max<std::ptrdiff_t>(_item.Count(), 0); }
		[[nodiscard]] inline std::ptrdiff_t Value() const { return _item.GetValue(); }

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
