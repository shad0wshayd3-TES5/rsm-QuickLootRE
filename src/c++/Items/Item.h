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

		Item(std::ptrdiff_t a_count, bool a_stealing, stl::observer<RE::InventoryEntryData*> a_item) :
			_item(a_count, a_stealing, a_item)
		{}

		Item(std::ptrdiff_t a_count, bool a_stealing, std::span<const RE::ObjectRefHandle> a_items) :
			_item(a_count, a_stealing, a_items)
		{}

		virtual ~Item() = default;

		Item& operator=(const Item&) = delete;
		Item& operator=(Item&&) = default;

		[[nodiscard]] int Compare(const Item& a_rhs) const { return _item.Compare(a_rhs._item); }

		[[nodiscard]] RE::GFxValue GFxValue(RE::GFxMovieView& a_view) const { return _item.GFxValue(a_view); }

		void Take(RE::Actor& a_dst, std::ptrdiff_t a_count) { DoTake(a_dst, a_count); }
		void Take(RE::Actor& a_dst) { DoTake(a_dst, 1); }
		void TakeAll(RE::Actor& a_dst) { DoTake(a_dst, Count()); }

		[[nodiscard]] double EnchantmentCharge() const { return _item.GetEnchantmentCharge(); }
		[[nodiscard]] std::ptrdiff_t Value() const { return _item.GetValue(); }
		[[nodiscard]] double Weight() const { return _item.GetWeight(); }

	protected:
		virtual void DoTake(RE::Actor& a_dst, std::ptrdiff_t a_count) = 0;

		[[nodiscard]] std::ptrdiff_t Count() const { return std::max<std::ptrdiff_t>(_item.Count(), 0); }
		[[nodiscard]] bool Stolen() const { return _item.IsStolen(); }

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
