#pragma once

#include <vector>

#include "ItemData.h"

#include "RE/Skyrim.h"


class InventoryList
{
public:
	using ItemList = std::vector<ItemData>;
	using size_type = typename ItemList::size_type;
	using reference = typename ItemList::reference;
	using const_reference = typename ItemList::const_reference;
	using iterator = typename ItemList::iterator;
	using const_iterator = typename ItemList::const_iterator;


	InventoryList() = default;
	~InventoryList() = default;

	void parse(RE::TESObjectREFR* a_ref);

	reference operator[](size_type a_pos);
	const_reference operator[](size_type a_pos) const;

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	bool empty() const noexcept;
	size_type size() const noexcept;

	void clear() noexcept;
	iterator erase(const_iterator a_pos);
	iterator erase(const_iterator a_first, const_iterator a_last);

private:
	static bool	is_valid(RE::TESBoundObject* a_item);


	ItemList _itemList;
};
